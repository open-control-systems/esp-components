idf_component_register(
    SRCS
    "async_task.cpp"
    "async_task_scheduler.cpp"
    "high_resolution_timer.cpp"
    "timer_store.cpp"
    "fanout_task.cpp"
    "async_func_scheduler.cpp"
    "async_func.cpp"
    "periodic_task_scheduler.cpp"
    "constant_delay_estimator.cpp"

    REQUIRES
    "freertos"
    "esp_timer"
    "ocs_core"

    INCLUDE_DIRS
    ".."
)
