/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_system.h"

#include "ocs_pipeline/version_json_formatter.h"

namespace ocs {
namespace pipeline {

VersionJsonFormatter::VersionJsonFormatter() {
    add("version_esp_idf", esp_get_idf_version());
}

} // namespace pipeline
} // namespace ocs
