cmake_minimum_required(VERSION 3.0)
if (UNIX)
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  set(CMAKE_EXPORT_COMPILE_COMMANDS yes)
endif()

project (blocks)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(THIRD_PARTY ${CMAKE_SOURCE_DIR}/third-party)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)

set(DIST ${CMAKE_BINARY_DIR}/dist)
set(DIST_BIN ${CMAKE_BINARY_DIR}/dist/bin)
set(DIST_LIB ${CMAKE_BINARY_DIR}/dist/lib)
set(DIST_JS ${CMAKE_BINARY_DIR}/dist/js)
set(DIST_RB ${CMAKE_BINARY_DIR}/dist/ruby)
file(MAKE_DIRECTORY ${DIST})
file(MAKE_DIRECTORY ${DIST_BIN})
file(MAKE_DIRECTORY ${DIST_LIB})
file(MAKE_DIRECTORY ${DIST_JS})
file(MAKE_DIRECTORY ${DIST_RB})

set(EXECUTABLE_OUTPUT_PATH ${DIST_BIN})
set(LIBRARY_OUTPUT_PATH ${DIST_LIB})
set(JS_OUTPUT_PATH ${DIST_JS})

set(BLOCKS_COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/include)
set(BLOCKS_COMMON_GENERATED_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/generated_include)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(Boost_USE_MULTITHREADED YES)
find_package(Boost 1.55 REQUIRED
  COMPONENTS
  system
  timer
  thread
  filesystem
  )

find_package(Panda REQUIRED)
find_package(Amqpcpp REQUIRED)

include(Livescript)

include_directories(
  ${BLOCKS_COMMON_INCLUDE_DIR}
  ${BLOCKS_COMMON_GENERATED_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

add_definitions(-fpermissive)
# add_definitions(-Wall)
# add_definitions(-stdlib)
#add_definitions(-O3)

if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

if (DEFINED SANITIZE)
  message("-- Using Clang's sanitizer: ${SANITIZE}")
  set(ASAN_FLAG -fsanitize=${SANITIZE})
  add_definitions(${ASAN_FLAG})
  add_definitions(-fno-omit-frame-pointer)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_FLAG}")
else()
  message("-- Not using Clang's sanitizer")
endif()

if (${PROFILING})
  add_definitions(-fprofile-instr-generate)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate")
endif (${PROFILING})

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server/chunk)
add_subdirectory(third-party)
