/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "esp_log.h"

#include "ocs_scheduler/async_task.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace scheduler {

namespace {

const char* log_tag = "async-task-scheduler";

} // namespace

AsyncTaskScheduler::TaskPtr AsyncTaskScheduler::add(ITask& task) {
    if (tasks_.size() == max_task_count) {
        return nullptr;
    }

    for (const auto& [e, t] : tasks_) {
        if (t == &task) {
            ESP_LOGE(log_tag, "task already exists: event=%lu", e);
            return nullptr;
        }
    }

    const ITask::Event event = BIT(tasks_.size());
    tasks_.emplace_back(std::pair<ITask::Event, ITask*>(event, &task));

    bits_all_ |= event;

    return TaskPtr(new (std::nothrow) AsyncTask(event_group_.get(), event));
}

void AsyncTaskScheduler::run() {
    ESP_LOGI(log_tag, "start handling tasks: count=%u max=%u", tasks_.size(),
             max_task_count);

    while (true) {
        wait();
    }
}

void AsyncTaskScheduler::wait(TickType_t wait) {
    configASSERT(tasks_.size());

    const EventBits_t bits =
        xEventGroupWaitBits(event_group_.get(), bits_all_, pdTRUE, pdFALSE, wait);

    for (auto& [event, task] : tasks_) {
        if (bits & event) {
            const auto code = task->run();
            if (code != status::StatusCode::OK) {
                ESP_LOGE(log_tag, "failed to run task: event=%lu code=%s", event,
                         status::code_to_str(code));
            }
        }
    }
}

} // namespace scheduler
} // namespace ocs
