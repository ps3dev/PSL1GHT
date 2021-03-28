if($ENV{PSL1GHT} STREQUAL "")
else()
    set(PSL1GHT $ENV{PSL1GHT})
endif()

add_custom_target(PS3Package)

macro(setupPSL1GHT target)
    if(NOT PSL1GHT)
        message(FATAL_ERROR "Location of PSL1GHT SDK not set")
    endif()
    set(CMAKE_C_COMPILER ${PSL1GHT}/ppu/bin/powerpc64-ps3-elf-gcc)
    set(CMAKE_CXX_COMPILER ${PSL1GHT}/ppu/bin/powerpc64-ps3-elf-g++)
    target_link_directories(${target} PUBLIC ${PSL1GHT}/ppu/lib ${PSL1GHT}/spu/lib ${PSL1GHT}/portlibs/ppu/lib)
    target_include_directories(${target} PUBLIC ${PSL1GHT}/ppu/include ${PSL1GHT}/spu/include ${PSL1GHT}/portlibs/ppu/include)
    target_compile_options(${target} PUBLIC -mcpu=cell)
    target_link_libraries(${target} PUBLIC gcm_sys rsx sysutil io)
    add_custom_command( TARGET ${target}
                        POST_BUILD
                        COMMAND echo Running PS3 specific post-build steps
                        COMMAND ${PSL1GHT}/ppu/bin/powerpc64-ps3-elf-strip $<TARGET_FILE:${target}> -o $<TARGET_FILE:${target}>.elf
                        COMMAND ${PSL1GHT}/bin/sprxlinker $<TARGET_FILE:${target}>.elf
                        COMMAND ${PSL1GHT}/bin/make_self $<TARGET_FILE:${target}>.elf $<TARGET_FILE:${target}>.self
                        COMMAND ${PSL1GHT}/bin/fself $<TARGET_FILE:${target}>.elf $<TARGET_FILE:${target}>.fake.self)
endmacro()

macro(setupPSL1GHTpkg_with_content target title appid sfoxml contentid icon contentfolder)
    if(NOT PSL1GHT)
        message(FATAL_ERROR "Location of PSL1GHT SDK not set")
    endif()

    spl1pkg(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})
    spl2pkg(${target} ${contentfolder})
    spl3pkg(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})

    add_dependencies(PS3Package ${target}_ps3_pkg_step3)
endmacro()

macro(setupPSL1GHTpkg target title appid sfoxml contentid icon)
    if(PSL1GHT STREQUAL "NOTSET")
        message(FATAL_ERROR "Location of PSL1GHT SDK not set")
    endif()

    spl1pkg(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})
    spl3pkg(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})

    add_dependencies(PS3Package ${target}_ps3_pkg_step3)
endmacro()

macro(spl1pkg target title icon appid sfoxml contentid)
    add_custom_target(${target}_ps3_pkg_step1 DEPENDS ${target})
    add_custom_command(TARGET ${target}_ps3_pkg_step1
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR
            COMMAND cp ${icon} ${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR/ICON0.PNG
            COMMAND ${PSL1GHT}/bin/make_self_npdrm "$<TARGET_FILE:${target}>.elf" "${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR/EBOOT.BIN" ${contentid}
            COMMAND ${PSL1GHT}/bin/sfo.py --title "${title}" --appid "${appid}" -f "${sfoxml}" ${CMAKE_BINARY_DIR}/${target}_pkg/PARAM.SFO)
endmacro()

macro(spl2pkg target title contentfolder)
    add_custom_target(${target}_ps3_pkg_step2 DEPENDS ${target}_ps3_pkg_step1)
    add_custom_command(TARGET ${target}_ps3_pkg_step2 COMMAND cd ${CMAKE_SOURCE_DIR}/${contentfolder} && cp * ${CMAKE_BINARY_DIR}/${target}_pkg/ && cd ../../../../PSL1GHT-CMake)
endmacro()

macro(spl3pkg target title icon appid sfoxml contentid)
    if (DEFINED ${target}_ps3_pkg_step2)
        add_custom_target(${target}_ps3_pkg_step3 DEPENDS ${target}_ps3_pkg_step2)
    else()
        add_custom_target(${target}_ps3_pkg_step3 DEPENDS ${target}_ps3_pkg_step1)
    endif()
    add_custom_command(TARGET ${target}_ps3_pkg_step3
            COMMAND ${PSL1GHT}/bin/pkg.py --contentid ${contentid} ${CMAKE_BINARY_DIR}/${target}_pkg/ ${CMAKE_BINARY_DIR}/${target}.pkg
            COMMAND cp ${CMAKE_BINARY_DIR}/${target}.pkg ${CMAKE_BINARY_DIR}/${target}.gnpdrm.pkg
            COMMAND ${PSL1GHT}/bin/package_finalize ${CMAKE_BINARY_DIR}/${target}.gnpdrm.pkg)
endmacro()
