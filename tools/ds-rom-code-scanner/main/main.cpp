/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

#include "ocs_core/log.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_io/default_gpio.h"
#include "ocs_onewire/bus.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_onewire/rom_code_scanner.h"
#include "ocs_onewire/serial_number_to_str.h"
#include "ocs_status/code_to_str.h"
#include "ocs_system/default_delayer.h"

using namespace ocs;

namespace {

const char* log_tag = "ds-rom-code-scanner";

struct ScanParams {
    gpio_num_t gpio { GPIO_NUM_NC };
};

void format_bus_params(fmt::json::CjsonObjectFormatter& formatter,
                       onewire::Bus::Params params) {
    configASSERT(
        formatter.add_number_cs("reset_pulse_interval", params.reset_pulse_interval));

    configASSERT(formatter.add_number_cs("presence_pulse_interval",
                                         params.presence_pulse_interval));

    configASSERT(
        formatter.add_number_cs("write_slot_interval", params.write_slot_interval));
    configASSERT(
        formatter.add_number_cs("write_bit_interval", params.write_bit_interval));

    configASSERT(formatter.add_number_cs("write_recovery_interval",
                                         params.write_recovery_interval));

    configASSERT(
        formatter.add_number_cs("read_slot_interval", params.read_slot_interval));
    configASSERT(
        formatter.add_number_cs("read_bit_init_interval", params.read_bit_init_interval));
    configASSERT(
        formatter.add_number_cs("read_bit_rc_interval", params.read_bit_rc_interval));

    configASSERT(
        formatter.add_number_cs("read_recovery_interval", params.read_recovery_interval));
}

void format_rom_code(cJSON* array,
                     fmt::json::CjsonUniqueBuilder& builder,
                     const onewire::RomCode& rom_code) {
    auto json = builder.make_object();
    configASSERT(json);

    fmt::json::CjsonObjectFormatter formatter(json.get());

    configASSERT(formatter.add_number_cs("family_code", rom_code.family_code));

    configASSERT(formatter.add_string_cs(
        "serial_number", onewire::serial_number_to_str(rom_code.serial_number).c_str()));

    configASSERT(formatter.add_number_cs("crc", rom_code.crc));

    configASSERT(cJSON_AddItemToArray(array, json.get()));
    json.release();
}

void scan_rom_codes(ScanParams scan_params, onewire::Bus::Params bus_params) {
    fmt::json::CjsonUniqueBuilder builder;

    auto json = builder.make_object();
    configASSERT(json);

    auto devices = cJSON_AddArrayToObject(json.get(), "devices");
    configASSERT(devices);

    fmt::json::CjsonObjectFormatter formatter(json.get());
    format_bus_params(formatter, bus_params);

    io::DefaultGpio gpio("test-GPIO-onewire-bus", scan_params.gpio);
    system::DefaultDelayer delayer;

    onewire::Bus bus(delayer, gpio, bus_params);

    onewire::RomCodeScanner scanner(bus);

    while (true) {
        onewire::RomCode rom_code;
        const auto code = scanner.scan(rom_code);
        if (code == status::StatusCode::NoData) {
            break;
        }

        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to scan ROM code: %s", status::code_to_str(code));
            return;
        }

        format_rom_code(devices, builder, rom_code);
    }

    fmt::json::DynamicFormatter json_formatter(
        CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_RESULT_BUFFER_SIZE);

    configASSERT(json_formatter.format(json.get()) == status::StatusCode::OK);

    ocs_logi(log_tag, "%s", json_formatter.c_str());
}

} // namespace

extern "C" void app_main(void) {
    scan_rom_codes(
        ScanParams {
            .gpio = static_cast<gpio_num_t>(CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_GPIO),
        },
        onewire::Bus::Params {
            .reset_pulse_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_RESET_PULSE_INTERVAL,
            .presence_pulse_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_PRESENCE_PULSE_INTERVAL,
            .write_slot_interval =
                core::Microsecond * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_WRITE_SLOT_INTERVAL,
            .write_bit_interval =
                core::Microsecond * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_WRITE_BIT_INTERVAL,
            .write_recovery_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_WRITE_RECOVERY_INTERVAL,
            .read_slot_interval =
                core::Microsecond * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_READ_SLOT_INTERVAL,
            .read_bit_init_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_READ_BIT_INIT_INTERVAL,
            .read_bit_rc_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_READ_BIT_RC_INTERVAL,
            .read_recovery_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_ROM_CODE_SCANNER_READ_RECOVERY_INTERVAL,
        });

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
