/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <unordered_map>

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

#include "unity.h"

#include "ocs_control/fsm_block.h"
#include "ocs_control/fsm_store.h"
#include "ocs_control/ifsm_handler.h"
#include "ocs_control/test/test_fsm_block_storage.h"
#include "ocs_core/noncopyable.h"
#include "ocs_test/test_clock.h"

namespace ocs {
namespace control {

namespace {

class TestHandler : public IFsmHandler, public core::NonCopyable<> {
public:
    TestHandler(status::StatusCode state_result = status::StatusCode::OK,
                status::StatusCode transit_result = status::StatusCode::OK)
        : state_result(state_result)
        , transit_result(transit_result) {
    }

    status::StatusCode handle_state() override {
        if (state_result != status::StatusCode::OK) {
            return state_result;
        }

        ++handle_state_count;

        return status::StatusCode::OK;
    }

    status::StatusCode handle_transit() override {
        if (transit_result != status::StatusCode::OK) {
            return transit_result;
        }

        ++handle_transit_count;

        return status::StatusCode::OK;
    }

    unsigned handle_state_count { 0 };
    unsigned handle_transit_count { 0 };

    status::StatusCode state_result { status::StatusCode::OK };
    status::StatusCode transit_result { status::StatusCode::OK };
};

enum class State {
    None = 0,
    Init = 1,
    Running = 2,
    Stopped = 3,
};

} // namespace

TEST_CASE("FSM store: initialization", "[ocs_control], [fsm_store]") {
    test::TestClock clock;
    TestFsmBlockStorage storage;
    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TEST_ASSERT_EQUAL(0, block.previous_state());
    TEST_ASSERT_EQUAL(0, block.current_state());
    TEST_ASSERT_EQUAL(0, block.next_state());
    TEST_ASSERT_EQUAL_INT64(0, block.previous_state_duration());
    TEST_ASSERT_EQUAL_INT64(0, block.current_state_duration());
}

TEST_CASE("FSM store: handle none state", "[ocs_control], [fsm_store]") {
    test::TestClock clock;
    TestFsmBlockStorage storage;
    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TestHandler handler;

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::None), handler));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());

    TEST_ASSERT_EQUAL(1, handler.handle_state_count);
}

TEST_CASE("FSM store: handle state: single state: different handlers",
          "[ocs_control], [fsm_store]") {
    test::TestClock clock;
    TestFsmBlockStorage storage;
    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TestHandler handler1;
    TestHandler handler2;

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Running), handler1));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Running), handler2));

    block.set_next(static_cast<FsmBlock::State>(State::Running));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler2.handle_state_count);
}

TEST_CASE("FSM store: handle state: multiple states: same handler",
          "[ocs_control], [fsm_store]") {
    test::TestClock clock;
    TestFsmBlockStorage storage;
    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TestHandler handler;

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Init), handler));
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Running), handler));

    block.set_next(static_cast<FsmBlock::State>(State::Init));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(1, handler.handle_state_count);

    block.set_next(static_cast<FsmBlock::State>(State::Running));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(2, handler.handle_state_count);
}

TEST_CASE("FSM store: handle state: multiple states: different handlers",
          "[ocs_control], [fsm_store]") {
    test::TestClock clock;
    TestFsmBlockStorage storage;
    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TestHandler handler1;
    TestHandler handler2;

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Init), handler1));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Running), handler2));

    block.set_next(static_cast<FsmBlock::State>(State::Init));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_state_count);

    block.set_next(static_cast<FsmBlock::State>(State::Running));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler2.handle_state_count);
}

TEST_CASE("FSM store: handle state: some handlers failed", "[ocs_control], [fsm_store]") {
    test::TestClock clock;
    TestFsmBlockStorage storage;
    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TestHandler handler1;
    TestHandler handler2(status::StatusCode::Error);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Init), handler1));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Init), handler2));

    block.set_next(static_cast<FsmBlock::State>(State::Init));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_state_count);
}

TEST_CASE("FSM store: handle state transition", "[ocs_control], [fsm_store]") {
    test::TestClock clock;
    TestFsmBlockStorage storage;
    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TestHandler handler1;
    TestHandler handler2;

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::None), handler1));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Init), handler2));

    // Test run to ensure current state handlers are called at least once.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);

    // Start the state transition.
    block.set_next(static_cast<FsmBlock::State>(State::Init));

    // Transition is in progress, continue calling handlers for the current state.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(2, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);

    // Finish the transition process.
    TEST_ASSERT_TRUE(block.is_in_transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_FALSE(block.is_in_transit());

    TEST_ASSERT_EQUAL(2, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(1, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);

    // One more time, to ensure correct handlers are called.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(2, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(2, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
}

TEST_CASE("FSM store: state transition: some handlers failed",
          "[ocs_control], [fsm_store]") {
    test::TestClock clock;
    TestFsmBlockStorage storage;
    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TestHandler handler1;
    TestHandler handler2(status::StatusCode::OK, status::StatusCode::InvalidArg);
    TestHandler handler3;

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::None), handler1));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Init), handler2));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Running), handler3));

    // Start the transition process.
    block.set_next(static_cast<FsmBlock::State>(State::Init));

    // Transition is in progress, continue calling handlers for the current state.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_transit_count);

    // Finish the transition process.
    TEST_ASSERT_TRUE(block.is_in_transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_FALSE(block.is_in_transit());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(1, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_transit_count);

    // Switch to the next state.
    block.set_next(static_cast<FsmBlock::State>(State::Running));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(2, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_transit_count);

    // Finish the transition process.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(2, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
    TEST_ASSERT_EQUAL(1, handler3.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_transit_count);
}

TEST_CASE("FSM store: state transition: block transition failed",
          "[ocs_control], [fsm_store]") {
    test::TestClock clock;

    TestFsmBlockStorage storage(status::StatusCode::NoMem, status::StatusCode::NoMem,
                                status::StatusCode::NoMem);

    const core::Time resolution = core::Duration::second;
    const char* block_id = "fsm_block_1";

    FsmBlock block(clock, storage, resolution, block_id);
    FsmStore store(block, "fsm_store_1");

    TestHandler handler1;
    TestHandler handler2(status::StatusCode::OK, status::StatusCode::InvalidArg);
    TestHandler handler3;

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::None), handler1));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Init), handler2));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      store.add(static_cast<FsmBlock::State>(State::Running), handler3));

    // Start the transition process.
    block.set_next(static_cast<FsmBlock::State>(State::Init));

    // Transition is in progress, continue calling handlers for the current state.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_transit_count);

    // Finish the transition process.
    TEST_ASSERT_TRUE(block.is_in_transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_FALSE(block.is_in_transit());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(1, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_transit_count);

    // Switch to the next state.
    block.set_next(static_cast<FsmBlock::State>(State::Running));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(2, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_transit_count);

    // Finish the transition process.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());

    TEST_ASSERT_EQUAL(1, handler1.handle_state_count);
    TEST_ASSERT_EQUAL(1, handler1.handle_transit_count);
    TEST_ASSERT_EQUAL(2, handler2.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler2.handle_transit_count);
    TEST_ASSERT_EQUAL(1, handler3.handle_state_count);
    TEST_ASSERT_EQUAL(0, handler3.handle_transit_count);
}

} // namespace control
} // namespace ocs
