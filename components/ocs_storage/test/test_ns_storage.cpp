/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>
#include <memory>

#include "unity.h"

#include "ocs_storage/flash_initializer.h"
#include "ocs_storage/ns_storage.h"

namespace ocs {
namespace storage {

TEST_CASE("NsStorage: write-read-erase", "[ocs_storage], [ns_storage]") {
    FlashInitializer initializer;

    const char* id = "foo";
    const unsigned write_value = 42;
    unsigned read_value = 0;

    NsStorage storage("tests");

    // Test writing data
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.write(id, &write_value, sizeof(write_value)));

    // Test reading the written data
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(id, &read_value, sizeof(read_value)));
    TEST_ASSERT_EQUAL(write_value, read_value);

    // Test erasing the data
    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.erase(id));

    // Test reading after erasing
    TEST_ASSERT_EQUAL(status::StatusCode::NoData,
                      storage.read(id, &read_value, sizeof(read_value)));
}

TEST_CASE("NsStorage: read: no data", "[ocs_storage], [ns_storage]") {
    FlashInitializer initializer;

    const char* id = "foo";
    unsigned value = 0;

    NsStorage storage("tests");
    TEST_ASSERT_EQUAL(status::StatusCode::NoData,
                      storage.read(id, &value, sizeof(value)));
}

TEST_CASE("NsStorage: erase: no data", "[ocs_storage], [ns_storage]") {
    FlashInitializer initializer;

    const char* id = "foo";

    NsStorage storage("tests");
    TEST_ASSERT_EQUAL(status::StatusCode::NoData, storage.erase(id));
}

TEST_CASE("NsStorage: write-read large value", "[ocs_storage], [ns_storage]") {
    FlashInitializer initializer;

    using Buffer = std::unique_ptr<uint8_t[]>;

    const char* id = "large";
    const size_t large_size = 1024;

    Buffer write_value(new (std::nothrow) uint8_t[large_size]);
    TEST_ASSERT_NOT_NULL(write_value);

    Buffer read_value(new (std::nothrow) uint8_t[large_size]);
    TEST_ASSERT_NOT_NULL(read_value);

    for (size_t i = 0; i < large_size; ++i) {
        write_value[i] = static_cast<uint8_t>(i % 256);
    }

    NsStorage storage("tests");

    // Test writing large data
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.write(id, write_value.get(), large_size));

    // Test reading the large data
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(id, read_value.get(), large_size));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(write_value.get(), read_value.get(), large_size);

    // Clean up
    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.erase(id));
}

TEST_CASE("NsStorage: overwrite key", "[ocs_storage], [ns_storage]") {
    FlashInitializer initializer;

    const char* id = "overwrite";
    const unsigned initial_value = 10;
    const unsigned updated_value = 20;
    unsigned read_value = 0;

    NsStorage storage("tests");

    // Write initial value
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.write(id, &initial_value, sizeof(initial_value)));

    // Overwrite with updated value
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.write(id, &updated_value, sizeof(updated_value)));

    // Read the updated value
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(id, &read_value, sizeof(read_value)));
    TEST_ASSERT_EQUAL(updated_value, read_value);

    // Clean up
    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.erase(id));
}

TEST_CASE("NsStorage: write-read string", "[ocs_storage], [ns_storage]") {
    FlashInitializer initializer;

    const char* id = "string";
    const char* write_str = "Hello, world!";

    char read_str[20];
    memset(read_str, 0, sizeof(read_str));

    NsStorage storage("tests");

    // Test writing string
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.write(id, write_str, strlen(write_str) + 1));

    // Test reading the string
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(id, read_str, strlen(write_str) + 1));
    TEST_ASSERT_EQUAL_STRING(write_str, read_str);

    // Clean up
    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.erase(id));
}

} // namespace storage
} // namespace ocs
