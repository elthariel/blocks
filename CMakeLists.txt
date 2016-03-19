cmake_minimum_required(VERSION 3.0)
if (UNIX)
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  set(CMAKE_EXPORT_COMPILE_COMMANDS yes)
endif()

project (blocks)


###############################################################################
### Definitions.

# A few top level variables
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(THIRD_PARTY ${CMAKE_SOURCE_DIR}/third-party)

# Create our distributable folder
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

# Tell cmake where to put produces files
set(EXECUTABLE_OUTPUT_PATH ${DIST_BIN})
set(LIBRARY_OUTPUT_PATH ${DIST_LIB})
set(JS_OUTPUT_PATH ${DIST_JS})


###############################################################################
### Dependencies

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(Boost_USE_MULTITHREADED YES)
find_package(Boost 1.55 REQUIRED
  COMPONENTS system timer thread filesystem )
find_package(Panda REQUIRED)
include(Livescript)


###############################################################################
### C++ build options

# Includes
set(BLOCKS_COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/include)
set(BLOCKS_COMMON_GENERATED_INCLUDE_DIR ${PROJECT_BINARY_DIR}/common/include)
set(BLOCKS_INCLUDE_DIRS
  ${BLOCKS_COMMON_INCLUDE_DIR}
  ${BLOCKS_COMMON_GENERATED_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  )

# Compile flags
set(BLOCKS_CXXFLAGS "-fpermissive -Wall -Wextra")
set(BLOCKS_LDFLAGS "")

if (DEFINED SANITIZE)
  message("-- Using Clang's sanitizer: ${SANITIZE}")
  set(ASAN_FLAG -fsanitize=${SANITIZE})
  set(BLOCKS_CXXFLAGS "${BLOCKS_CXXFLAGS} ${ASAN_FLAG} -fno-omit-frame-pointer")
  set(BLOCKS_LDFLAGS "${BLOCKS_LDFLAGS} ${ASAN_FLAG}")
else()
  message("-- Not using Clang's sanitizer")
endif()

if (${PROFILING})
  set(PROFILING_FLAG -fprofile-instr-generate)
  set(BLOCKS_CXXFLAGS "${BLOCKS_CXXFLAGS} ${PROFILING_FLAG}")
  set(BLOCKS_LDFLAGS "${BLOCKS_LDFLAGS} ${PROFILING_FLAG}")
endif (${PROFILING})


# Default to Debug Build
if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
    "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(third-party)
