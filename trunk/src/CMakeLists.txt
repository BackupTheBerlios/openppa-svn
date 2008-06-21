# Project name
project(OpenPPA)

# Build our testing hello world plugin
# add_subdirectory(HelloWorldPlugin)

# Then includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# sources
set(CoreFiles
	core/pcctrlnode.cpp
	core/pcpipectrl.cpp
	core/pcprocctrl.cpp
	core/pcpacketcenter.cpp)

set(ProxyFiles
	proxy/ppaproxy.cpp
	proxy/ppfunccenter.cpp
	proxy/ppfuncpacket.cpp)

set(ProxyGenFiles
	proxy/proxygen.cpp)

set(ProxyExecGenFiles
	proxy/proxyexecgen.cpp)

set(MiscFiles
	misc/dbgprint.cpp)


# Hello world
set(HelloWorldExecFiles
	HelloWorldPlugin/main.c)

set(HelloWorldLibFiles
	HelloWorldPlugin/plugin/plugin.c)

# compilation

# PPAified HelloWorldExec
add_executable(helloWorldExec
	${HelloWorldExecFiles})

# link PPAExec with library, so it can be called directly
add_executable(PPAExec
	${CoreFiles}
	${ProxyFiles}
	${MiscFiles}
	${HelloWorldLibFiles}
	${ProxyExecGenFiles})

# PPA shared lib
add_library(PPALib SHARED
	${CoreFiles}
	${ProxyFiles}
	${MiscFiles}
	${ProxyGenFiles})


target_link_libraries(PPAExec pthread)
target_link_libraries(PPALib pthread)

# link those helloworld
target_link_libraries(helloWorldExec PPALib)
