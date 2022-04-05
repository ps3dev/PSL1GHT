cmake_minimum_required(VERSION 3.0)

project(test_cxx C)

include($ENV{PS3DEV}/contrib/ppu-helpers.cmake)

add_executable(test_c test.c)

add_spu_project(${CMAKE_CURRENT_SOURCE_DIR}/spu test_spu)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/content/test_spu.elf
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/test_spu.elf
    COMMENT "Copying SPU Executable to content folder"
    COMMAND cmake -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/content/
    COMMAND cmake -E copy ${CMAKE_CURRENT_BINARY_DIR}/test_spu.elf ${CMAKE_CURRENT_SOURCE_DIR}/content/
)

add_ps3_build_steps(test_c)
setup_ps3_pkg(test_c "Untitled C SPU" UNTITSPU1 ${PS3DEV_DIR}/bin/sfo.xml UP0001-UNTITLXX1_00-0000000000000000 ${PS3DEV_DIR}/bin/ICON0.PNG ${CMAKE_CURRENT_SOURCE_DIR}/content)

target_link_libraries(test_c rsx gcm_sys io sysutil m)

add_custom_target(
    copy_contents ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/content/test_spu.elf
)

add_dependencies(test_c_pkg copy_contents)
