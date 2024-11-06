/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vector>

#include "soc/soc_caps.h"
#include "unity.h"

#include "ocs_io/adc/default_store.h"

namespace ocs {
namespace io {
namespace adc {

TEST_CASE("Default ADC store: register maximum number of ADC",
          "[ocs_io], [adc], [default_store]") {
    const std::vector<adc_unit_t> units {
        ADC_UNIT_1,
        ADC_UNIT_2,
    };

    for (const auto& unit : units) {
        DefaultStore store(DefaultStore::Params {
            .unit = unit,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_10,
        });

        const unsigned count = SOC_ADC_CHANNEL_NUM(unit);

        for (unsigned n = 0; n < count; ++n) {
            const Channel channel = static_cast<Channel>(n);
            TEST_ASSERT_NOT_NULL(store.add(channel));
        }
    }
}

TEST_CASE("Default ADC store: register the same ADC twice",
          "[ocs_io], [adc], [default_store]") {
    const std::vector<adc_unit_t> units {
        ADC_UNIT_1,
        ADC_UNIT_2,
    };

    for (const auto& unit : units) {
        DefaultStore store(DefaultStore::Params {
            .unit = unit,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_10,
        });

        const Channel channel = ADC_CHANNEL_5;
        TEST_ASSERT_NOT_NULL(store.add(channel));
        TEST_ASSERT_NULL(store.add(channel));
    }
}

TEST_CASE("Default ADC store: register overflow", "[ocs_io], [adc], [default_store]") {
    const std::vector<adc_unit_t> units {
        ADC_UNIT_1,
        ADC_UNIT_2,
    };

    for (const auto& unit : units) {
        DefaultStore store(DefaultStore::Params {
            .unit = unit,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_10,
        });

        const unsigned count = SOC_ADC_CHANNEL_NUM(unit);

        for (unsigned n = 0; n < count; ++n) {
            const Channel channel = static_cast<Channel>(n);
            TEST_ASSERT_NOT_NULL(store.add(channel));
        }

        const Channel channel = static_cast<Channel>(count);
        TEST_ASSERT_NULL(store.add(channel));
    }
}

TEST_CASE("Default ADC store: read/convert operations",
          "[ocs_io], [adc], [default_store]") {
    const std::vector<adc_unit_t> units {
        ADC_UNIT_1,
        ADC_UNIT_2,
    };

    for (const auto& unit : units) {
        DefaultStore store(DefaultStore::Params {
            .unit = unit,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_10,
        });

        const Channel channel = ADC_CHANNEL_5;

        auto adc = store.add(channel);
        TEST_ASSERT_NOT_NULL(adc);

        const auto read_result = adc->read();
        TEST_ASSERT_EQUAL(status::StatusCode::OK, read_result.code);

        const auto conv_result = adc->convert(read_result.value);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, conv_result.code);
    }
}

} // namespace adc
} // namespace io
} // namespace ocs
