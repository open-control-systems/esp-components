/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_algo/storage_ops.h"
#include "ocs_test/test_storage.h"

namespace ocs {
namespace algo {

namespace {

using TestStorage = test::TestStorage<unsigned>;

} // namespace

TEST_CASE("Storage Ops: prob read: empty storage", "[ocs_algo], [storage_ops]") {
    const char* key = "id";

    TestStorage storage;

    unsigned recv_value = 0;

    TEST_ASSERT_EQUAL(
        status::StatusCode::NoData,
        StorageOps::prob_read(storage, key, &recv_value, sizeof(recv_value)));

    TEST_ASSERT_EQUAL(0, recv_value);
}

TEST_CASE("Storage Ops: prob read: invalid key", "[ocs_algo], [storage_ops]") {
    const char* key = "foo";
    const char* invalid_key = "bar";
    const unsigned value = 42;

    TestStorage storage;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.write(key, &value, sizeof(value)));

    unsigned size = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.probe(key, size));
    TEST_ASSERT_EQUAL(size, sizeof(value));

    unsigned recv_value = 0;

    TEST_ASSERT_EQUAL(
        status::StatusCode::NoData,
        StorageOps::prob_read(storage, invalid_key, &recv_value, sizeof(recv_value)));

    TEST_ASSERT_EQUAL(0, recv_value);
}

TEST_CASE("Storage Ops: prob read: size mismatch", "[ocs_algo], [storage_ops]") {
    const char* key = "foo";
    const unsigned value = 42;

    TestStorage storage;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.write(key, &value, sizeof(value)));

    unsigned recv_value = 0;

    TEST_ASSERT_EQUAL(
        status::StatusCode::InvalidArg,
        StorageOps::prob_read(storage, key, &recv_value, sizeof(recv_value) - 1));

    TEST_ASSERT_EQUAL(0, recv_value);
}

TEST_CASE("Storage Ops: prob read: prob failed", "[ocs_algo], [storage_ops]") {
    const char* key = "foo";
    const unsigned value = 42;

    TestStorage storage;
    storage.probe_status = status::StatusCode::Error;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.write(key, &value, sizeof(value)));

    unsigned recv_value = 0;

    TEST_ASSERT_EQUAL(
        storage.probe_status,
        StorageOps::prob_read(storage, key, &recv_value, sizeof(recv_value) - 1));

    TEST_ASSERT_EQUAL(0, recv_value);
}

TEST_CASE("Storage Ops: prob read: read failed", "[ocs_algo], [storage_ops]") {
    const char* key = "foo";
    const unsigned value = 42;

    TestStorage storage;
    storage.read_status = status::StatusCode::Timeout;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.write(key, &value, sizeof(value)));

    unsigned recv_value = 0;

    TEST_ASSERT_EQUAL(
        storage.read_status,
        StorageOps::prob_read(storage, key, &recv_value, sizeof(recv_value)));

    TEST_ASSERT_EQUAL(0, recv_value);
}

TEST_CASE("Storage Ops: prob read: properly read", "[ocs_algo], [storage_ops]") {
    const char* key = "foo";
    const unsigned value = 42;

    TestStorage storage;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.write(key, &value, sizeof(value)));

    unsigned recv_value = 0;

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        StorageOps::prob_read(storage, key, &recv_value, sizeof(recv_value)));

    TEST_ASSERT_EQUAL(value, recv_value);
}

} // namespace algo
} // namespace ocs
