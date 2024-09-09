idf_component_register(
    SRCS
    "test_fanout_task.cpp"
    "test_timer_store.cpp"
    "test_async_task.cpp"
    "test_async_task_scheduler.cpp"
    "test_high_resolution_timer.cpp"
    "test_async_func_scheduler.cpp"

    REQUIRES
    "unity"
    "ocs_scheduler"
)
