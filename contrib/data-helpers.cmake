enable_language(ASM)

function(add_bin_file input_file header_name target array_name)
    add_custom_command(
        OUTPUT ${header_name}.h ${header_name}.s
        COMMENT "Generating ${header_name}.h from ${input_file}"
        DEPENDS ${input_file}
        COMMAND ${PS3DEV_DIR}/bin/raw2h ${input_file} ${header_name}.h ${header_name}.s ${array_name}
    )
    target_sources(${target} PRIVATE ${header_name}.s)
    target_include_directories(${target} PRIVATE ${CMAKE_BINARY_DIR})
endfunction()
