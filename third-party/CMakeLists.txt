add_subdirectory(duktape)

set(ENTITYX_BUILD_SHARED 0)
set(ENTITYX_BUILD_TESTING 0)
add_subdirectory(entityx)

if(APPLE)
  set(OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/1.0.2g)
endif()
add_subdirectory(rabbitmq)
