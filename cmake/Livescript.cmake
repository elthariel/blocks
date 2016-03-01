
function(compile_livescript target)
  file(GLOB_RECURSE LS_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.ls)

  foreach(LS ${LS_FILES})
    get_filename_component(BASENAME ${LS} NAME_WE)
    get_filename_component(FOLDER ${LS} DIRECTORY)
    set(OUT_FOLDER ${JS_OUTPUT_PATH}/${FOLDER})
    set(OUT ${OUT_FOLDER}/${BASENAME}.js)
    list(APPEND ${target}_JS_FILES ${OUT})

    message("-- Livescript file ${LS}")
    add_custom_command(
      OUTPUT ${OUT}
      COMMAND /usr/local/bin/lsc -co ${OUT_FOLDER} ${CMAKE_CURRENT_SOURCE_DIR}/${LS}
      DEPENDS ${LS}
      COMMENT "Compiling ${LS}"
    )

  set(${NAME}_JS_FILES ${${NAME}_JS_FILES} PARENT_SCOPE)
  endforeach()

  add_custom_target(${target}_compile_ls ALL DEPENDS ${${target}_JS_FILES})
endfunction()
