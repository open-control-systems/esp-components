/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

#include "ocs_core/log.h"
#include "ocs_scheduler/periodic_task_scheduler.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace scheduler {

PeriodicTaskScheduler::PeriodicTaskScheduler(core::IClock& clock,
                                             IDelayEstimator& estimator,
                                             const char* id,
                                             unsigned max_count)
    : max_count_(max_count)
    , log_tag_(id)
    , clock_(clock)
    , estimator_(estimator) {
    configASSERT(max_count_);
}

unsigned PeriodicTaskScheduler::max_count() const {
    return max_count_;
}

unsigned PeriodicTaskScheduler::count() const {
    return nodes_.size();
}

status::StatusCode
PeriodicTaskScheduler::add(ITask& task, const char* id, core::microseconds_t interval) {
    configASSERT(id);
    configASSERT(interval > 0);
    configASSERT(interval >= core::Millisecond);

    if (nodes_.size() == max_count()) {
        return status::StatusCode::Error;
    }

    for (auto& node : nodes_) {
        if (strcmp(node->id(), id) == 0) {
            return status::StatusCode::InvalidArg;
        }
    }

    task_min_interval_ = std::min(task_min_interval_, interval);

    NodePtr node(new (std::nothrow) Node(clock_, task, id, interval));
    configASSERT(node);

    nodes_.push_back(node);

    return status::StatusCode::OK;
}

status::StatusCode PeriodicTaskScheduler::start() {
    ocs_logi(log_tag_.c_str(),
             "start handling tasks: count=%u/%u task_min_interval=%lli(ms)", count(),
             max_count(), task_min_interval_ / core::Millisecond);

    return status::StatusCode::OK;
}

status::StatusCode PeriodicTaskScheduler::run() {
    estimator_.begin();

    const auto start_ts = clock_.now();
    run_();
    const auto total_ts = clock_.now() - start_ts;

    total_ts_min_ = std::min(total_ts_min_, total_ts);
    total_ts_max_ = std::max(total_ts_max_, total_ts);

    const auto estimated_delay = estimator_.estimate();

    ocs_logd(log_tag_.c_str(),
             "delay estimating: total=%lli(usec) total_min=%lli(usec) "
             "total_max=%lli(usec) task_min=%lli(ms) estimated=%lu(ms)",
             total_ts, total_ts_min_, total_ts_max_,
             task_min_interval_ / core::Millisecond, pdTICKS_TO_MS(estimated_delay));

    vTaskDelay(estimated_delay);

    return status::StatusCode::OK;
}

void PeriodicTaskScheduler::run_() {
    for (auto& node : nodes_) {
        const auto code = node->run();
        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag_.c_str(), "failed to run task: id=%s code=%s", node->id(),
                     status::code_to_str(code));
        }
    }
}

PeriodicTaskScheduler::Node::Node(core::IClock& clock,
                                  ITask& task,
                                  const char* id,
                                  core::microseconds_t interval)
    : id_(id)
    , limiter_(clock, interval)
    , task_(&task) {
}

status::StatusCode PeriodicTaskScheduler::Node::run() {
    return limiter_.allow() ? task_->run() : status::StatusCode::OK;
}

const char* PeriodicTaskScheduler::Node::id() const {
    return id_.c_str();
}

} // namespace scheduler
} // namespace ocs
