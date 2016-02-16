cmake_minimum_required(VERSION 3.0)

project(blocks_server)

set(SERVER_HEADERS
	./include/TcpServer.hh
	./include/Player.hh
	./include/Server.hh
	./include/Chunk.hh
	../common/include/CircularBuffer.hh
)

set(SERVER_SOURCES
	./src/main.cc
	./src/TcpServer.cc
	./src/Player.cc
	./src/Server.cc
	./src/Chunk.cc
	../common/src/CircularBuffer.cpp
)


if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")


include_directories(
	${PROJECT_SOURCE_DIR}/include
	/usr/include/python2.7
)

add_executable(blocks_server ${SERVER_HEADERS} ${SERVER_SOURCES})

set_target_properties(blocks_server PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(blocks_server common
 boost_system boost_thread
	python2.7
 ${LINUX_LIBS}
)

# Here to enable C++11 features, we should make a better use of this :)
# target_compile_features(blocks_server PRIVATE cxx_rvalue_references)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(UNIX)
	install(TARGETS blocks_server
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
endif(UNIX)
