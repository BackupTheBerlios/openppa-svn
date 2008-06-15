# Project name
project(HelloWorldPlugin)

add_subdirectory(plugin)

link_directories(${PLUGIN_BINARY_DIR}/Plugin)
add_executable(pluginHello main.c)

target_link_libraries(pluginHello Plugin)
