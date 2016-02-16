
cmake_minimum_required(VERSION 3.0)

project(common)

set(COMMON_FBS
  ./idl/chunk.fbs
)

set(COMMON_HEADERS
  ./include/constants.hh
  ./include/map.hh
  ./include/position.hh
  ./include/chunk.hh
  ./include/meta.hh
  ./include/multi_array.hh
)

set(COMMON_SOURCES
  ./src/map.cc
  ./src/position.cc
  ./src/chunk.cc
  )

file(GLOB COMMON_TESTS ./tests/*.cc)

find_path(FLATBUFFERS_HOME
  /usr/local
  NAMES
  bin/flatc
  PATHS
)

if(FLATBUFFERS_HOME-NOTFOUND)
  message(SEND_ERROR "flatbuffers not found. Please install it")
endif(FLATBUFFERS_HOME-NOTFOUND)

set(FLATC "${FLATBUFFERS_HOME}/bin/flatc")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/generated_include)
foreach(FBS_FILE ${COMMON_FBS})
   get_filename_component(FBS_NAME ${FBS_FILE} NAME_WE)
   set(OUT_NAME ${CMAKE_CURRENT_SOURCE_DIR}/generated_include/${FBS_NAME}_generated.h)
   list(APPEND COMMON_GENERATED_HEADERS ${OUT_NAME})

   message("${OUT_NAME}")

   add_custom_command(
     PRE_BUILD
     OUTPUT ${OUT_NAME}
     COMMAND ${FLATC} -b -c --gen-mutable --raw-binary -o ${CMAKE_CURRENT_SOURCE_DIR}/generated_include/ ${CMAKE_CURRENT_SOURCE_DIR}/${FBS_FILE}
     DEPENDS ${FBS_FILE}
     COMMENT "Compiling ${FBS_FILE}"
   )
endforeach(FBS_FILE)

include_directories(
  ${FLATBUFFERS_HOME}/include
)

add_library(common STATIC
  ${COMMON_GENERATED_HEADERS} ${COMMON_HEADERS} ${COMMON_SOURCES}
)

# Build tests !
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#set(BUILD_SHARED_LIBRARIES OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static")
# add_executable(common_tests ${COMMON_TESTS})
# target_link_libraries(common_tests common boost_unit_test_framework)
