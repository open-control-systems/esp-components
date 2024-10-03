/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "unity.h"

#include "ocs_control/test/test_fsm_block_storage.h"

namespace ocs {
namespace control {

TestFsmBlockStorage::TestFsmBlockStorage(status::StatusCode read_status,
                                         status::StatusCode write_status,
                                         status::StatusCode erase_status)
    : read_status(read_status)
    , write_status(write_status)
    , erase_status(erase_status) {
}

status::StatusCode TestFsmBlockStorage::read(const char* key, void* data, unsigned size) {
    if (read_status != status::StatusCode::OK) {
        return read_status;
    }

    if (strcmp(key, "prev_state") == 0) {
        TEST_ASSERT_EQUAL(sizeof(prev_state), size);
        *static_cast<FsmBlock::State*>(data) = prev_state;
    } else if (strcmp(key, "curr_state") == 0) {
        TEST_ASSERT_EQUAL(sizeof(curr_state), size);
        *static_cast<FsmBlock::State*>(data) = curr_state;
    } else if (strcmp(key, "prev_state_dur") == 0) {
        TEST_ASSERT_EQUAL(sizeof(prev_state_duration), size);
        *static_cast<core::microseconds_t*>(data) = prev_state_duration;
    } else if (strcmp(key, "curr_state_dur") == 0) {
        TEST_ASSERT_EQUAL(sizeof(curr_state_duration), size);
        *static_cast<core::microseconds_t*>(data) = curr_state_duration;
    } else if (strcmp(key, "write_count") == 0) {
        TEST_ASSERT_EQUAL(sizeof(write_count), size);
        *static_cast<decltype(write_count)*>(data) = write_count;
    } else {
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode
TestFsmBlockStorage::write(const char* key, const void* data, unsigned size) {
    if (write_status != status::StatusCode::OK) {
        return write_status;
    }

    if (strcmp(key, "prev_state") == 0) {
        TEST_ASSERT_EQUAL(sizeof(prev_state), size);
        prev_state = *static_cast<const FsmBlock::State*>(data);
    } else if (strcmp(key, "curr_state") == 0) {
        TEST_ASSERT_EQUAL(sizeof(curr_state), size);
        curr_state = *static_cast<const FsmBlock::State*>(data);
    } else if (strcmp(key, "prev_state_dur") == 0) {
        TEST_ASSERT_EQUAL(sizeof(prev_state_duration), size);
        prev_state_duration = *static_cast<const core::microseconds_t*>(data);
    } else if (strcmp(key, "curr_state_dur") == 0) {
        TEST_ASSERT_EQUAL(sizeof(curr_state_duration), size);
        curr_state_duration = *static_cast<const core::microseconds_t*>(data);
    } else if (strcmp(key, "write_count") == 0) {
        TEST_ASSERT_EQUAL(sizeof(write_count), size);
        write_count = *static_cast<const decltype(write_count)*>(data);
    } else {
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode TestFsmBlockStorage::erase(const char* key) {
    if (erase_status != status::StatusCode::OK) {
        return erase_status;
    }

    if (strcmp(key, "prev_state") == 0) {
        prev_state = 0;
    } else if (strcmp(key, "curr_state") == 0) {
        curr_state = 0;
    } else if (strcmp(key, "prev_state_dur") == 0) {
        prev_state_duration = 0;
    } else if (strcmp(key, "curr_state_dur") == 0) {
        curr_state_duration = 0;
    } else if (strcmp(key, "write_count") == 0) {
        write_count = 0;
    } else {
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace control
} // namespace ocs
