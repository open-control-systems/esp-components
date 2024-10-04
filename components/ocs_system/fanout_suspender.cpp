/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"
#include "ocs_system/fanout_suspender.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "fanout_suspender";

} // namespace

status::StatusCode FanoutSuspender::suspend() {
    for (auto& node : nodes_) {
        const auto code = node.handle_suspend();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag, "failed to handle suspend: id=%s code=%s", node.id(),
                     status::code_to_str(code));
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode FanoutSuspender::resume() {
    for (auto& node : nodes_) {
        const auto code = node.handle_resume();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag, "failed to handle resume: id=%s code=%s", node.id(),
                     status::code_to_str(code));
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode FanoutSuspender::add(ISuspendHandler& handler, const char* id) {
    for (const auto& node : nodes_) {
        if (strcmp(node.id(), id) == 0) {
            return status::StatusCode::InvalidArg;
        }
    }

    Node node(handler, id);
    nodes_.push_back(node);

    return status::StatusCode::OK;
}

status::StatusCode FanoutSuspender::Node::handle_suspend() {
    return handler_.handle_suspend();
}

status::StatusCode FanoutSuspender::Node::handle_resume() {
    return handler_.handle_resume();
}

const char* FanoutSuspender::Node::id() const {
    return id_.c_str();
}

} // namespace system
} // namespace ocs
