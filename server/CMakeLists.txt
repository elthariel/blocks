cmake_minimum_required(VERSION 3.0)

project(blocks_server)


###############################################################################
### Sources

file(GLOB SERVER_HEADERS ./include/*.hh)
file(GLOB SERVER_SOURCES ./src/*.cc)


###############################################################################
### Configure Build

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${BLOCKS_INCLUDE_DIRS}
  SYSTEM ${PANDA_INCLUDE_DIRS}
  SYSTEM ${THIRD_PARTY}/easylogging
)

link_directories(
  ${PANDA_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BLOCKS_CXXFLAGS}")
add_definitions(-std=c++14)


###############################################################################
### Build

add_executable(blocks_server ${SERVER_HEADERS} ${SERVER_SOURCES})
set_target_properties(blocks_server PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(blocks_server
  ${BLOCKS_LDFLAGS}
  ${PANDA_LIBRARIES}
  ${Boost_LIBRARIES}
  ${LINUX_LIBS}
  common
)
