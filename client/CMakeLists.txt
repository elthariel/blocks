cmake_minimum_required(VERSION 3.0)

project(blocks_client)

file(GLOB_RECURSE CLIENT_HEADERS ./include/ *.hh)
file(GLOB_RECURSE CLIENT_SOURCES ./src/ *.cc)

set(ENV{PKG_CONFIG_PATH} "$ENV{HOME}/usr/local/lib64/pkgconfig/:$ENV{PKG_CONFIG_PATH}")
find_package(PkgConfig REQUIRED)
pkg_check_modules(BULLET bullet REQUIRED)


include_directories(
  ${PROJECT_SOURCE_DIR}/include
  SYSTEM ${PANDA_INCLUDE_DIRS}
  SYSTEM ${BULLET_INCLUDE_DIRS}
  SYSTEM ${AMQPCPP_INCLUDE_DIRS}
  SYSTEM ${THIRD_PARTY}/entityx
  SYSTEM ${THIRD_PARTY}/duktape
)

link_directories(
  ${PANDA_LIBRARY_DIRS}
  ${BULLET_LIBRARY_DIRS}
  ${AMQPCPP_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS}
)


add_definitions(-std=c++14)

add_executable(blocks_client ${CLIENT_HEADERS} ${CLIENT_SOURCES})
set_target_properties(blocks_client PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(blocks_client
  common
  entityx
  ${PANDA_LIBRARIES}
  ${BULLET_LIBRARIES}
  ${AMQPCPP_LIBRARIES}
  ${Boost_LIBRARIES}
  ${LINUX_LIBS}
  mruby
)

add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/client/ruby
        ${DIST_RB}/client
  COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/media
        ${DIST}/media
)
