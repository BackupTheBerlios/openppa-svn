# Project name
project(OpenPPA)

# PPACoreExec and PPACoreLib
add_executable(PPACoreExec core/ppacore.c)
add_library(PPACoreLib SHARED core/ppacore.c)

# PPAProxy 'executable'
add_executable(PPAProxy proxy/ppaproxy.c)
target_link_libraries(PPAProxy PPACoreLib)
add_dependencies(PPAProxy PPACoreLib)
