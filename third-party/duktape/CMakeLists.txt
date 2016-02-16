cmake_minimum_required(VERSION 3.0)

project(duktape)

set(DUKTAPE_HEADERS
   ./duktape.h)

set(DUKTAPE_SOURCES
  ./duktape.c)

add_library(${PROJECT_NAME} STATIC
  ${DUKTAPE_HEADERS} ${DUKTAPE_SOURCES}
)
