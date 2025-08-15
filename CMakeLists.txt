zephyr_include_directories(include)
zephyr_include_directories(${APPLICATION_SOURCE_DIR}/include)
zephyr_library()
add_subdirectory(${ZEPHYR_CURRENT_MODULE_DIR}/drivers/display)