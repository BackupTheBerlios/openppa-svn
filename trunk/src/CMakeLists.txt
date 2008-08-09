# Project name
project(OpenPPA)

# Build our testing hello world plugin
# add_subdirectory(HelloWorldPlugin)

# Then includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# sources
set(CoreFiles
	core/pcplugentity.cpp
	core/pcpipectrl.cpp
	core/pcprocctrl.cpp
	core/pcpacketcenter.cpp)

set(ProxyFiles
	proxy/ppappsender.cpp
	proxy/ppappreceiver.cpp
	proxy/ppcoreplugbase.cpp

	proxy/varpack/ppvarpack.cpp
	proxy/varpack/ppclspack.cpp
	proxy/varpack/ppptrpack.cpp
	proxy/varpack/ppfunpack.cpp
	proxy/ppcallpack.cpp)

set(MiscFiles
	misc/dbgprint.cpp)



# link PPAExec with library, so it can be called directly
add_executable(PPACoreExec
	${CoreFiles}
	${MiscFiles})

add_library(PPACoreLib SHARED
	${CoreFiles}
	${MiscFiles})

add_executable(PPAProxyAppLib		# add_library!
	${ProxyFiles}
	${MiscFiles}
)

add_library(PPAProxyPlugLib SHARED
	${ProxyFiles}
	${MiscFiles}
)

target_link_libraries(PPACoreExec dl)
target_link_libraries(PPACoreLib dl)
target_link_libraries(PPAProxyAppLib dl)
target_link_libraries(PPAProxyPlugLib dl)
