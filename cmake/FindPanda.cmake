#
# Cmake Module to find Panda3d libs
#

find_path(PANDA_INCLUDE
  panda.h
  HINTS
  ${CMAKE_SOURCE_DIR}/../panda3d/built/include
  ENV PANDA_INCLUDE
  NO_CMAKE_SYSTEM_PATH
  )

find_library(LIBPANDA
  panda
  HINTS
  ${CMAKE_SOURCE_DIR}/../panda3d/built/lib
  ENV LIBPANDA
  NO_CMAKE_SYSTEM_PATH
  # Add yours here
  )

message("-- Panda 3D framework:")

if(PANDA_INCLUDE-NOTFOUND)
  message("---- PANDA_INCLUDE: NOT FOUND")
else(PANDA_INCLUDE-NOTFOUND)
  message("---- Found PANDA_INCLUDE: ${PANDA_INCLUDE}")
endif(PANDA_INCLUDE-NOTFOUND)

if(LIBPANDA-NOTFOUND)
  message("---- LIBPANDA: NOT FOUND")
else(LIBPANDA-NOTFOUND)
  message("---- Found LIBPANDA: ${LIBPANDA}")
endif(LIBPANDA-NOTFOUND)

if (PANDA_INCLUDE-NOTFOUND OR LIBPANDA-NOTFOUND)
  set(PANDA_FOUND no)
else (PANDA_INCLUDE-NOTFOUND OR LIBPANDA-NOTFOUND)
  SET(PANDA_FOUND yes)
  SET(PANDA_INCLUDE_DIRS
    ${PANDA_INCLUDE}
    /usr/include/python2.7
    /usr/include/eigen3)
  get_filename_component(PANDA_LIBRARY_DIRS ${LIBPANDA} DIRECTORY)
  SET(PANDA_LIBRARIES
    panda
    pandafx
    pandaexpress
    p3framework
    p3dtoolconfig
    p3dtool
    p3direct
    python2.7
    )

endif ()
