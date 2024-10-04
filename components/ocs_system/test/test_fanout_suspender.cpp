/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_system/fanout_suspender.h"

namespace ocs {
namespace system {

namespace {

class TestHandler : public ISuspendHandler {
public:
    TestHandler(status::StatusCode suspend_code, status::StatusCode resume_code)
        : suspend_code_(suspend_code)
        , resume_code_(resume_code) {
    }

    status::StatusCode handle_suspend() override {
        ++suspend_count;

        return suspend_code_;
    }

    status::StatusCode handle_resume() override {
        ++resume_count;

        return resume_code_;
    }

    unsigned suspend_count { 0 };
    unsigned resume_count { 0 };

private:
    status::StatusCode suspend_code_ { status::StatusCode::OK };
    status::StatusCode resume_code_ { status::StatusCode::OK };
};

} // namespace

TEST_CASE("Fanout suspender: suspend/resume component: all succeeded",
          "[ocs_system], [fanout_suspender]") {
    FanoutSuspender suspender;
    TestHandler handler1(status::StatusCode::OK, status::StatusCode::OK);
    TestHandler handler2(status::StatusCode::OK, status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.add(handler1, "handler_1"));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.add(handler2, "handler_2"));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.suspend());
    TEST_ASSERT_EQUAL(1, handler1.suspend_count);
    TEST_ASSERT_EQUAL(1, handler2.suspend_count);
    TEST_ASSERT_EQUAL(0, handler1.resume_count);
    TEST_ASSERT_EQUAL(0, handler2.resume_count);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.resume());
    TEST_ASSERT_EQUAL(1, handler1.suspend_count);
    TEST_ASSERT_EQUAL(1, handler2.suspend_count);
    TEST_ASSERT_EQUAL(1, handler1.resume_count);
    TEST_ASSERT_EQUAL(1, handler2.resume_count);
}

TEST_CASE("Fanout suspender: suspend/resume component: one failed",
          "[ocs_system], [fanout_suspender]") {
    FanoutSuspender suspender;
    TestHandler handler1(status::StatusCode::OK, status::StatusCode::Error);
    TestHandler handler2(status::StatusCode::Error, status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.add(handler1, "handler_1"));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.add(handler2, "handler_2"));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.suspend());
    TEST_ASSERT_EQUAL(1, handler1.suspend_count);
    TEST_ASSERT_EQUAL(1, handler2.suspend_count);
    TEST_ASSERT_EQUAL(0, handler1.resume_count);
    TEST_ASSERT_EQUAL(0, handler2.resume_count);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.resume());
    TEST_ASSERT_EQUAL(1, handler1.suspend_count);
    TEST_ASSERT_EQUAL(1, handler2.suspend_count);
    TEST_ASSERT_EQUAL(1, handler1.resume_count);
    TEST_ASSERT_EQUAL(1, handler2.resume_count);
}

TEST_CASE("Fanout suspender: add same component twice",
          "[ocs_system], [fanout_suspender]") {
    FanoutSuspender suspender;
    TestHandler handler(status::StatusCode::OK, status::StatusCode::Error);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, suspender.add(handler, "handler"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg, suspender.add(handler, "handler"));
}

} // namespace system
} // namespace ocs
