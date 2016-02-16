
cmake_minimum_required(VERSION 3.0)
if (UNIX)
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

project (blocks)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(THIRD_PARTY ${CMAKE_SOURCE_DIR}/third-party)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(BLOCKS_COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/include)
set(BLOCKS_COMMON_GENERATED_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/generated_include)

set(Boost_USE_MULTITHREADED YES)
find_package(Boost 1.58 REQUIRED
  COMPONENTS
  system
  timer
  thread
  )

include_directories(
  ${BLOCKS_COMMON_INCLUDE_DIR}
  ${BLOCKS_COMMON_GENERATED_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
add_definitions(-fpermissive)
#add_definitions(-O0)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(third-party)
