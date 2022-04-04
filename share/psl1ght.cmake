if(NOT $ENV{PS3DEV} STREQUAL "")
    set(PS3DEV_DIR $ENV{PS3DEV})
endif()

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

function(setup_ps3_pkg target title appid sfoxml contentid icon contentfolder)
    psl1ght_pkg_step1(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})
    psl1ght_pkg_step2(${target} ${contentfolder})
    psl1ght_pkg_step3(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})

    add_dependencies(${target}_pkg ${target}_ps3_pkg_step3)
endfunction()

function(setup_ps3_pkg_nc target title appid sfoxml contentid icon)
    psl1ght_pkg_step1(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})
    psl1ght_pkg_step3(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})

    add_dependencies(${target}_pkg ${target}_ps3_pkg_step3)
endfunction()

function(psl1ght_pkg_step1 target title icon appid sfoxml contentid)
    add_custom_target(${target}_pkg)

    add_custom_target(${target}_ps3_pkg_step1 DEPENDS ${target})
    add_custom_command(TARGET ${target}_ps3_pkg_step1
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR
            COMMAND cp ${icon} ${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR/ICON0.PNG
            COMMAND ${PS3DEV_DIR}/bin/make_self_npdrm "$<TARGET_FILE:${target}>.elf" "${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR/EBOOT.BIN" ${contentid}
            COMMAND ${PS3DEV_DIR}/bin/sfo.py --title "${title}" --appid "${appid}" -f "${sfoxml}" ${CMAKE_BINARY_DIR}/${target}_pkg/PARAM.SFO)
endfunction()

function(psl1ght_pkg_step2 target title contentfolder)
    add_custom_target(${target}_ps3_pkg_step2 DEPENDS ${target}_ps3_pkg_step1)
    add_custom_command(TARGET ${target}_ps3_pkg_step2 COMMAND cd ${CMAKE_SOURCE_DIR}/${contentfolder} && cp * ${CMAKE_BINARY_DIR}/${target}_pkg/)
endfunction()

function(psl1ght_pkg_step3 target title icon appid sfoxml contentid)
    if (DEFINED ${target}_ps3_pkg_step2)
        add_custom_target(${target}_ps3_pkg_step3 DEPENDS ${target}_ps3_pkg_step2)
    else()
        add_custom_target(${target}_ps3_pkg_step3 DEPENDS ${target}_ps3_pkg_step1)
    endif()
    add_custom_command(TARGET ${target}_ps3_pkg_step3
            COMMAND ${PS3DEV_DIR}/bin/pkg.py --contentid ${contentid} ${CMAKE_BINARY_DIR}/${target}_pkg/ ${CMAKE_BINARY_DIR}/${target}.pkg
            COMMAND cp ${CMAKE_BINARY_DIR}/${target}.pkg ${CMAKE_BINARY_DIR}/${target}.gnpdrm.pkg
            COMMAND ${PS3DEV_DIR}/bin/package_finalize ${CMAKE_BINARY_DIR}/${target}.gnpdrm.pkg)
endfunction()
