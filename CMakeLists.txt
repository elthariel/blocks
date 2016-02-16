
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_OSX_SYSROOT "/Library/Frameworks/MacOSX10.7.sdk")

if(APPLE)
  # set(CMAKE_OSX_DEPLOYMENT_TARGET "10.7")
  # set(CMAKE_OSX_SYSROOT "/Library/Frameworks/MacOSX10.7.sdk")
  # set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk")
endif(APPLE)

project (blocks)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(BLOCKS_COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/include)
set(BLOCKS_COMMON_GENERATED_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/generated_include)

#
# Let's find a local Panda sdk / install
#
find_path(PANDA_HOME
  NAMES
  include/panda3d/panda.h
  include/panda.h
  PATHS
  "$ENV{HOME}/code/cxx/panda3d/built"
  "/data/prog/cpp/panda3d/built"
  "$ENV{HOME}/usr/local/panda"
  /Developer/Panda3D
  /usr/local
  /usr
  # Add yours here
)

message(${PANDA_HOME})

if(PANDA_HOME-NOTFOUND)
	message(SEND_ERROR "Unable to find PANDA_HOME")
endif()

if (UNIX AND NOT APPLE) #i.e. champii
  set(LINUX_LIBS pthread)
  set(PANDA_INCLUDE_DIRS
    ${PANDA_HOME}/include
   /usr/include/eigen3)
endif(UNIX AND NOT APPLE)
if (APPLE)
  set(PANDA_INCLUDE_DIRS
    ${PANDA_HOME}/include/panda3d)
endif(APPLE)

message("${PANDA_INCLUDE_DIRS}")

include_directories(
  ${BLOCKS_COMMON_INCLUDE_DIR}
  ${BLOCKS_COMMON_GENERATED_INCLUDE_DIR}
  ${PANDA_INCLUDE_DIRS}
)

add_definitions(-std=c++14)
add_definitions(-fpermissive)
#add_definitions(-O0)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
