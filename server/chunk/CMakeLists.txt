cmake_minimum_required(VERSION 3.0)

project(blocks_server)

file(GLOB SERVER_HEADERS ./include/*.hh)
file(GLOB SERVER_SOURCES ./src/*.cc)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  SYSTEM ${PANDA_INCLUDE_DIRS}
  SYSTEM ${AMQPCPP_INCLUDE_DIRS}
)

link_directories(
  ${PANDA_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS}
)

add_definitions(-std=c++14)
add_executable(blocks_server ${SERVER_HEADERS} ${SERVER_SOURCES})
set_target_properties(blocks_server PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(blocks_server
  common
  ${PANDA_LIBRARIES}
  ${Boost_LIBRARIES}
  ${AMQPCPP_LIBRARIES}
  ${LINUX_LIBS}
)
