cmake_minimum_required(VERSION 3.0)

project(common)


###############################################################################
### Sources

file(GLOB COMMON_HEADERS ./include/*.hh)
file(GLOB COMMON_SOURCES ./src/*.cc)
file(GLOB COMMON_TESTS ./tests/*.cc)
file(GLOB COMMON_FBS ./idl/*.fbs)

find_package(FlatBuffers REQUIRED)


###############################################################################
### Compile FlatBuffers IDL.

# XXX: Move that to cmake/ folder
set(FBS_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${FBS_INCLUDE_DIR})

foreach(FBS_FILE ${COMMON_FBS})
  get_filename_component(FBS_NAME ${FBS_FILE} NAME)
  get_filename_component(FBS_NAME_WE ${FBS_NAME} NAME_WE)
  set(OUT_NAME ${FBS_INCLUDE_DIR}/${FBS_NAME_WE}_generated.h)
  list(APPEND COMMON_GENERATED_HEADERS ${OUT_NAME})

  add_custom_command(
    PRE_BUILD
    OUTPUT ${OUT_NAME}
    COMMAND ${FLATBUFFERS_COMPILER} -b -c --gen-mutable --raw-binary -o ${FBS_INCLUDE_DIR} ${FBS_FILE}
    DEPENDS ${FBS_FILE}
    COMMENT "Compiling ${FBS_FILE}"
    )
endforeach(FBS_FILE)


###############################################################################
### Configure build

include_directories(
  ${BLOCKS_INCLUDE_DIRS}
  ${FLATBUFFERS_INCLUDE_DIRS}
  ${THIRD_PARTY}/duktape
  )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BLOCKS_CXXFLAGS}")
add_definitions(-std=c++14)


###############################################################################
### Build

add_library(common STATIC
  ${COMMON_GENERATED_HEADERS}
  ${COMMON_HEADERS}
  ${COMMON_SOURCES}
  )
target_link_libraries(${BLOCKS_LDFLAGS} common duktape)

if (UNIX)
  add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/common/ruby
        ${DIST_RB}/common
    )
endif()
