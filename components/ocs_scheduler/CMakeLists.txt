idf_component_register(
    SRCS
    "async_task.cpp"
    "async_task_scheduler.cpp"
    "high_resolution_timer.cpp"
    "timer_store.cpp"
    "fanout_task.cpp"

    REQUIRES
    "freertos"
    "esp_timer"

    INCLUDE_DIRS
    ".."
)
