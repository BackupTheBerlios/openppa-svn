# Project name
project(OpenPPA)

#sources
set(PPACoreExecFiles
	core/pcctrlnode.cpp
	core/pcpipectrl.cpp
	core/pcprocctrl.cpp
	core/pcpacketfactory.cpp

	misc/dbgprint.cpp
)

set(PPACoreLibFiles
	core/pcctrlnode.cpp
	core/pcpipectrl.cpp
	core/pcprocctrl.cpp
	core/pcpacketfactory.cpp

	misc/dbgprint.cpp
)

set(PPAProxyFiles
	proxy/ppaproxy.cpp

	misc/dbgprint.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# PPACoreExec and PPACoreLib
add_executable(PPACoreExec ${PPACoreExecFiles})
add_library(PPACoreLib SHARED ${PPACoreLibFiles})

# PPAProxy 'executable'
add_executable(PPAProxy ${PPAProxyFiles})
target_link_libraries(PPAProxy PPACoreLib)
target_link_libraries(PPACoreExec pthread)
target_link_libraries(PPACoreLib pthread)
add_dependencies(PPAProxy PPACoreLib)
