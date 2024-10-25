/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_control/fsm_block.h"
#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace control {

class TestFsmBlockStorage : public storage::IStorage, public core::NonCopyable<> {
public:
    TestFsmBlockStorage(status::StatusCode read_status = status::StatusCode::OK,
                        status::StatusCode write_status = status::StatusCode::OK,
                        status::StatusCode erase_status = status::StatusCode::OK);

    status::StatusCode read(const char* key, void* data, unsigned size) override;
    status::StatusCode write(const char* key, const void* data, unsigned size) override;
    status::StatusCode erase(const char* key) override;

    FsmBlock::State prev_state { 0 };
    FsmBlock::State curr_state { 0 };
    core::Time prev_state_duration { 0 };
    core::Time curr_state_duration { 0 };
    uint64_t write_count { 0 };

    status::StatusCode read_status { status::StatusCode::OK };
    status::StatusCode write_status { status::StatusCode::OK };
    status::StatusCode erase_status { status::StatusCode::OK };
};

} // namespace control
} // namespace ocs
