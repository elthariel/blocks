cmake_minimum_required(VERSION 3.0)

project(blocks_client)

file(GLOB CLIENT_HEADERS ./include/*.hh)
file(GLOB CLIENT_SOURCES ./src/*.cc)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")


include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PANDA_INCLUDE_DIRS}
  ${THIRD_PARTY}/entity
)

link_directories(
  ${PANDA_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS}
)

add_definitions(-std=c++14)
add_executable(blocks_client ${CLIENT_HEADERS} ${CLIENT_SOURCES})

set_target_properties(blocks_client PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(blocks_client
  common
  ${PANDA_LIBRARIES}
  ${Boost_LIBRARIES}
  ${LINUX_LIBS}
)

# Here to enable C++11 features, we should make a better use of this :)
# target_compile_features(blocks_client PRIVATE cxx_rvalue_references)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)


if(UNIX)

  install(TARGETS blocks_client
	RUNTIME DESTINATION bin
	CONFIGURATIONS All)

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
	DESTINATION ./
	CONFIGURATIONS Release RelWithDebInfo Debug)

  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ln -sf ${CMAKE_SOURCE_DIR}/media/textures ${CMAKE_CURRENT_BINARY_DIR}/dist/media/textures
    )

endif(UNIX)
