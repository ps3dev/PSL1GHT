cmake_minimum_required(VERSION 3.0)

project(test_c C)

set(TITLE "Test C + SPU + CMake PSL1GHT")
set(TITLE_ID "UNTITSPU1")
set(VERSION "01.00")
set(LICENSE "MIT LICENSE")

include($ENV{PS3DEV}/contrib/ppu-helpers.cmake)
include(${PS3DEV_DIR}/contrib/data-helpers.cmake)

add_executable(test_c test.c)

add_spu_project(${CMAKE_CURRENT_SOURCE_DIR}/spu test_spu)

add_ps3_build_steps(test_c)
setup_ps3_pkg(test_c ${TITLE} ${TITLE_ID} UP0001-${TITLE_ID}_00-0000000000000000 ${PS3DEV_DIR}/bin/ICON0.PNG content/)

target_link_libraries(test_c rsx gcm_sys io sysutil m)

add_bin_file(${CMAKE_CURRENT_BINARY_DIR}/test_spu.elf spu_data test_c spu_elf)

