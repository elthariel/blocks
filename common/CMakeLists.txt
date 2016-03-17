
cmake_minimum_required(VERSION 3.0)

project(common)

set(COMMON_FBS
  ./idl/chunk.fbs
)

file(GLOB COMMON_HEADERS ./include/*.hh)
file(GLOB COMMON_SOURCES ./src/*.cc)
file(GLOB COMMON_TESTS ./tests/*.cc)

find_package(FlatBuffers REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/generated_include)
foreach(FBS_FILE ${COMMON_FBS})
   get_filename_component(FBS_NAME ${FBS_FILE} NAME_WE)
   set(OUT_NAME_CPP ${CMAKE_CURRENT_SOURCE_DIR}/generated_include/${FBS_NAME}_generated.h)
   set(OUT_NAME_JS ${CMAKE_CURRENT_SOURCE_DIR}/generated_include/${FBS_NAME}_generated.js)
   list(APPEND COMMON_GENERATED_HEADERS ${OUT_NAME_CPP})

   add_custom_command(
     PRE_BUILD
     OUTPUT ${OUT_NAME_CPP}
     COMMAND ${FLATBUFFERS_COMPILER} -b -c -s --gen-mutable --raw-binary -o ${CMAKE_CURRENT_SOURCE_DIR}/generated_include/ ${CMAKE_CURRENT_SOURCE_DIR}/${FBS_FILE}
     DEPENDS ${FBS_FILE}
     COMMENT "Compiling ${FBS_FILE}"
   )
endforeach(FBS_FILE)

include_directories(
  ${FLATBUFFERS_INCLUDE_DIRS}
  ${THIRD_PARTY}/duktape
  )

add_definitions(-std=c++14)
add_library(common STATIC
  ${COMMON_GENERATED_HEADERS} ${COMMON_HEADERS} ${COMMON_SOURCES}
)
target_link_libraries(common duktape)

add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ln -sfT ${CMAKE_SOURCE_DIR}/ruby ${DIST_RB}/common
)

# Build tests !
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#set(BUILD_SHARED_LIBRARIES OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static")
# add_executable(common_tests ${COMMON_TESTS})
# target_link_libraries(common_tests common boost_unit_test_framework)
