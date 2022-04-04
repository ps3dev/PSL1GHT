if(NOT PS3DEV_DIR)
    message(FATAL_ERROR "Location of PSL1GHT SDK not set")
endif()

function(add_ps3_build_steps target)
        add_custom_command(
                        TARGET ${target}
                        POST_BUILD
                        COMMAND echo Running PS3 specific post-build steps
                        COMMAND ${PS3DEV_DIR}/ppu/bin/powerpc64-ps3-elf-strip $<TARGET_FILE:${target}> -o $<TARGET_FILE:${target}>.elf
                        COMMAND ${PS3DEV_DIR}/bin/sprxlinker $<TARGET_FILE:${target}>.elf
                        COMMAND ${PS3DEV_DIR}/bin/make_self $<TARGET_FILE:${target}>.elf $<TARGET_FILE:${target}>.self
                        COMMAND ${PS3DEV_DIR}/bin/fself $<TARGET_FILE:${target}>.elf $<TARGET_FILE:${target}>.fake.self
       )
endfunction()

function(setupPSL1GHTpkg_with_content target title appid sfoxml contentid icon contentfolder)
    psl1ght_pkg_step1(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})
    psl1ght_pkg_step2(${target} ${contentfolder})
    psl1ght_pkg_step3(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})

    add_dependencies(PS3Package ${target}_ps3_pkg_step3)
endfunction()

function(setupPSL1GHTpkg target title appid sfoxml contentid icon)
    psl1ght_pkg_step1(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})
    psl1ght_pkg_step3(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})

    add_dependencies(PS3Package ${target}_ps3_pkg_step3)
endfunction()

function(psl1ght_pkg_step1 target title icon appid sfoxml contentid)
    add_custom_target(${target}_ps3_pkg_step1 DEPENDS ${target})
    add_custom_command(TARGET ${target}_ps3_pkg_step1
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR
            COMMAND cp ${icon} ${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR/ICON0.PNG
            COMMAND ${PSL1GHT}/bin/make_self_npdrm "$<TARGET_FILE:${target}>.elf" "${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR/EBOOT.BIN" ${contentid}
            COMMAND ${PSL1GHT}/bin/sfo.py --title "${title}" --appid "${appid}" -f "${sfoxml}" ${CMAKE_BINARY_DIR}/${target}_pkg/PARAM.SFO)
endfunction()

function(psl1ght_pkg_step2 target title contentfolder)
    add_custom_target(${target}_ps3_pkg_step2 DEPENDS ${target}_ps3_pkg_step1)
    add_custom_command(TARGET ${target}_ps3_pkg_step2 COMMAND cd ${CMAKE_SOURCE_DIR}/${contentfolder} && cp * ${CMAKE_BINARY_DIR}/${target}_pkg/ && cd ../../../../PSL1GHT-CMake)
endfunction()

function(psl1ght_pkg_step3 target title icon appid sfoxml contentid)
    if (DEFINED ${target}_ps3_pkg_step2)
        add_custom_target(${target}_ps3_pkg_step3 DEPENDS ${target}_ps3_pkg_step2)
    else()
        add_custom_target(${target}_ps3_pkg_step3 DEPENDS ${target}_ps3_pkg_step1)
    endif()
    add_custom_command(TARGET ${target}_ps3_pkg_step3
            COMMAND ${PSL1GHT}/bin/pkg.py --contentid ${contentid} ${CMAKE_BINARY_DIR}/${target}_pkg/ ${CMAKE_BINARY_DIR}/${target}.pkg
            COMMAND cp ${CMAKE_BINARY_DIR}/${target}.pkg ${CMAKE_BINARY_DIR}/${target}.gnpdrm.pkg
            COMMAND ${PSL1GHT}/bin/package_finalize ${CMAKE_BINARY_DIR}/${target}.gnpdrm.pkg)
endfunction()
