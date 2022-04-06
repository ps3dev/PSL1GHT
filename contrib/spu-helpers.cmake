function(add_spu_dirs target)
    target_include_directories(${target} PUBLIC ${PS3DEV_DIR}/spu/include/)
    target_link_directories(${target} PUBLIC ${PS3DEV_DIR}/spu/lib/)
endfunction()
