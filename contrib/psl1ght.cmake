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
            COMMENT "Running PS3 specific post-build steps"
            COMMAND ${PS3DEV_DIR}/ppu/bin/powerpc64-ps3-elf-strip $<TARGET_FILE:${target}> -o $<TARGET_FILE:${target}>.elf # Strip the binary
            COMMAND ${PS3DEV_DIR}/bin/sprxlinker $<TARGET_FILE:${target}>.elf # Run sprxlinker on it
            COMMAND ${PS3DEV_DIR}/bin/make_self $<TARGET_FILE:${target}>.elf $<TARGET_FILE:${target}>.self # Convert it into a .self file
            COMMAND ${PS3DEV_DIR}/bin/fself $<TARGET_FILE:${target}>.elf $<TARGET_FILE:${target}>.fake.self # Convert the .self file info a .fake.self
       )
endfunction()

# Setup PS3 Package Generation
function(setup_ps3_pkg target title appid sfoxml contentid icon contentfolder)
    psl1ght_pkg_step1(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})
    psl1ght_pkg_step2(${target} ${contentfolder})
    psl1ght_pkg_step3(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})

    add_dependencies(${target}_pkg ${target}_ps3_pkg_step3)
endfunction()

# Setup PS3 Package Generation with no content (nc)
function(setup_ps3_pkg_nc target title appid sfoxml contentid icon)
    psl1ght_pkg_step1(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})
    psl1ght_pkg_step3(${target} ${title} ${icon} ${appid} ${sfoxml} ${contentid})

    add_dependencies(${target}_pkg ${target}_ps3_pkg_step3)
endfunction()

# This function makes the staging directory, copies the icon into it, makes the .self into a npdrm self (EBOOT.BIN) and generates the PARAM.SFO
function(psl1ght_pkg_step1 target title icon appid sfoxml contentid)
    add_custom_target(${target}_pkg)

    add_custom_target(${target}_ps3_pkg_step1 DEPENDS ${target})
    add_custom_command(
        TARGET ${target}_ps3_pkg_step1
        COMMENT "Running initial PKG setup"
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR # Make the staging directory
        COMMAND cp ${icon} ${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR/ICON0.PNG # Copy the icon into the staging directory
        COMMAND ${PS3DEV_DIR}/bin/make_self_npdrm "$<TARGET_FILE:${target}>.elf" "${CMAKE_BINARY_DIR}/${target}_pkg/USRDIR/EBOOT.BIN" ${contentid} # Make the MNDRM self file
        COMMAND ${PS3DEV_DIR}/bin/sfo.py --title "${title}" --appid "${appid}" -f "${sfoxml}" ${CMAKE_BINARY_DIR}/${target}_pkg/PARAM.SFO # Generate the PARAM.SFO from the given sfo.xml file
    )
endfunction()

# This function just adds a custom command to copy content from a specific folder (the 2nd parameter) to the pkg staging folder
function(psl1ght_pkg_step2 target contentfolder)
    add_custom_target(${target}_ps3_pkg_step2 DEPENDS ${target}_ps3_pkg_step1)
    add_custom_command(
        TARGET ${target}_ps3_pkg_step2
        COMMENT "Adding content files"
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/${contentfolder}/* ${CMAKE_BINARY_DIR}/${target}_pkg/ # Copy the content into the staging directory
    )
endfunction()

# This function builds the PKG before finalizing it for gnpdrm
function(psl1ght_pkg_step3 target title icon appid sfoxml contentid)
    if (DEFINED ${target}_ps3_pkg_step2)
        add_custom_target(${target}_ps3_pkg_step3 DEPENDS ${target}_ps3_pkg_step2)
    else()
        add_custom_target(${target}_ps3_pkg_step3 DEPENDS ${target}_ps3_pkg_step1)
    endif()

    add_custom_command(
        TARGET ${target}_ps3_pkg_step3
        COMMENT "Finalizing PKG build"
        COMMAND ${PS3DEV_DIR}/bin/pkg.py --contentid ${contentid} ${CMAKE_BINARY_DIR}/${target}_pkg/ ${CMAKE_BINARY_DIR}/${target}.pkg # Create the PKG file
        COMMAND cp ${CMAKE_BINARY_DIR}/${target}.pkg ${CMAKE_BINARY_DIR}/${target}.gnpdrm.pkg # Copy that PKG file to a new PKG file for finalization
        COMMAND ${PS3DEV_DIR}/bin/package_finalize ${CMAKE_BINARY_DIR}/${target}.gnpdrm.pkg # Finalize that PKG file to get a GNPDRM PKG
    )
endfunction()

# This function just adds a custom command to compile a vertex shader
function(add_vertex_shader file)
    add_custom_command(
        OUTPUT ${file}.vcg
        COMMAND cgcomp -v ${file} # Run cgcomp on the input
    )
endfunction()

# This function just adds a custom command to compile a fragment shader
function(add_fragment_shader file)
    add_custom_command(
        OUTPUT ${file}.fcg
        COMMAND cgcomp -f ${file} # Run cgcomp on the input
    )
endfunction()
