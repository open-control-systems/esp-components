/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>
#include <cstring>
#include <deque>
#include <string>

#include "unity.h"

#include "ocs_core/stream_transceiver.h"

namespace ocs {
namespace core {

namespace {

struct TestStreamReader : public IStreamReader {
    using Data = std::deque<std::string>;

    TestStreamReader() = default;

    TestStreamReader(Data data)
        : data_(data) {
    }

    status::StatusCode begin() override {
        ++begin_count;
        return begin_status;
    }

    status::StatusCode end() override {
        ++end_count;
        return end_status;
    }

    status::StatusCode cancel() override {
        ++cancel_count;
        return cancel_status;
    }

    status::StatusCode read(void* data, unsigned& size) override {
        ++read_count;

        if (read_status != status::StatusCode::OK) {
            return read_status;
        }

        if (!data_.size()) {
            return status::StatusCode::NoData;
        }

        size = std::min(size, data_.back().size());

        memcpy(data, data_.front().data(), size);
        data_.pop_front();

        return status::StatusCode::OK;
    }

    status::StatusCode begin_status { status::StatusCode::OK };
    status::StatusCode end_status { status::StatusCode::OK };
    status::StatusCode cancel_status { status::StatusCode::OK };
    status::StatusCode read_status { status::StatusCode::OK };

    unsigned begin_count { 0 };
    unsigned end_count { 0 };
    unsigned cancel_count { 0 };
    unsigned read_count { 0 };

private:
    Data data_;
};

struct TestStreamWriter : public IStreamWriter {
    status::StatusCode begin() override {
        ++begin_count;
        return begin_status;
    }

    status::StatusCode end() override {
        ++end_count;
        return end_status;
    }

    status::StatusCode cancel() override {
        ++cancel_count;
        return cancel_status;
    }

    status::StatusCode write(const void* data, unsigned size) override {
        ++write_count;
        if (write_status != status::StatusCode::OK) {
            return write_status;
        }

        const char* ptr = static_cast<const char*>(data);

        for (unsigned n = 0; n < size; ++n) {
            buffer_.push_back(ptr[n]);
        }

        return status::StatusCode::OK;
    }

    const StreamTransceiver::Buffer get_buffer() {
        return buffer_;
    }

    status::StatusCode begin_status { status::StatusCode::OK };
    status::StatusCode end_status { status::StatusCode::OK };
    status::StatusCode cancel_status { status::StatusCode::OK };
    status::StatusCode write_status { status::StatusCode::OK };

    unsigned begin_count { 0 };
    unsigned end_count { 0 };
    unsigned cancel_count { 0 };
    unsigned write_count { 0 };

private:
    StreamTransceiver::Buffer buffer_;
};

} // namespace

TEST_CASE("Stream transceiver: automatically end process",
          "[ocs_core], [stream_transceiver]") {
    TestStreamReader reader;
    TestStreamWriter writer;
    StreamTransceiver::Buffer buffer;

    {
        StreamTransceiver transceiver(reader, writer, buffer);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, transceiver.transceive());
    }

    TEST_ASSERT_EQUAL(1, reader.end_count);
    TEST_ASSERT_EQUAL(1, writer.end_count);
}

TEST_CASE("Stream transceiver: reader begin failed", "[ocs_core], [stream_transceiver]") {
    TestStreamReader reader;
    reader.begin_status = status::StatusCode::Error;

    TestStreamWriter writer;
    StreamTransceiver::Buffer buffer;

    StreamTransceiver transceiver(reader, writer, buffer);
    TEST_ASSERT_EQUAL(reader.begin_status, transceiver.transceive());

    TEST_ASSERT_EQUAL(1, reader.begin_count);
    TEST_ASSERT_EQUAL(0, reader.end_count);
    TEST_ASSERT_EQUAL(0, reader.cancel_count);
    TEST_ASSERT_EQUAL(0, reader.read_count);

    TEST_ASSERT_EQUAL(0, writer.begin_count);
    TEST_ASSERT_EQUAL(0, writer.end_count);
    TEST_ASSERT_EQUAL(0, writer.cancel_count);
    TEST_ASSERT_EQUAL(0, writer.write_count);
}

TEST_CASE("Stream transceiver: writer begin failed", "[ocs_core], [stream_transceiver]") {
    TestStreamReader reader;

    TestStreamWriter writer;
    writer.begin_status = status::StatusCode::Timeout;

    StreamTransceiver::Buffer buffer;

    StreamTransceiver transceiver(reader, writer, buffer);
    TEST_ASSERT_EQUAL(writer.begin_status, transceiver.transceive());

    TEST_ASSERT_EQUAL(1, reader.begin_count);
    TEST_ASSERT_EQUAL(0, reader.end_count);
    TEST_ASSERT_EQUAL(0, reader.cancel_count);
    TEST_ASSERT_EQUAL(0, reader.read_count);

    TEST_ASSERT_EQUAL(1, writer.begin_count);
    TEST_ASSERT_EQUAL(0, writer.end_count);
    TEST_ASSERT_EQUAL(0, writer.cancel_count);
    TEST_ASSERT_EQUAL(0, writer.write_count);
}

TEST_CASE("Stream transceiver: transceive data", "[ocs_core], [stream_transceiver]") {
    const char* want_string = "helloworld";
    StreamTransceiver::Buffer buffer;
    buffer.resize(strlen(want_string));

    TestStreamReader reader({ "hello", "world" });
    TestStreamWriter writer;

    StreamTransceiver transceiver(reader, writer, buffer);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, transceiver.transceive());
    TEST_ASSERT_EQUAL_STRING_LEN("helloworld", writer.get_buffer().data(),
                                 buffer.capacity());
}

} // namespace core
} // namespace ocs
