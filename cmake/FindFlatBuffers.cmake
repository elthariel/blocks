
#
# Let's find flat buffer
#

message("-- Flatbuffers:")

find_path(FLATBUFFERS_HOME
  NAMES
  bin/flatc
  PATHS
  /usr/local
)

if(FLATBUFFERS_HOME-NOTFOUND)
  message("---- flatbuffers NOT FOUND. Please install it")
else(FLATBUFFERS_HOME-NOTFOUND)
  set(FLATBUFFERS_FOUND YES)
  set(FLATBUFFERS_COMPILER ${FLATBUFFERS_HOME}/bin/flatc)
  set(FLATBUFFERS_INCLUDE_DIRS ${FLATBUFFERS_HOME}/include)
  message("---- Found compiler: ${FLATBUFFERS_COMPILER}")
  message("---- Found include: ${FLATBUFFERS_INCLUDE_DIRS}")
endif(FLATBUFFERS_HOME-NOTFOUND)
