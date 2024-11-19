/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_security/basic_sha_engine.h"

namespace ocs {
namespace security {

BasicShaEngine::BasicShaEngine(IShaEngine::Algorithm algorithm)
    : algorithm_(algorithm) {
}

IShaEngine::Algorithm BasicShaEngine::get_algorithm() const {
    return algorithm_;
}

} // namespace security
} // namespace ocs
