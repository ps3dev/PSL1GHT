cmake_minimum_required(VERSION 3.0)

project(test C)

set(TITLE "Test C CMake PSL1GHT")
set(TITLE_ID "UNTITLED1")
set(VERSION "0.0.1")
set(LICENSE "MIT LICENSE")

include($ENV{PS3DEV}/contrib/ppu-helpers.cmake)

add_executable(test_c test.c)
add_ps3_build_steps(test_c)
setup_ps3_pkg_nc(test_c ${TITLE} ${TITLE_ID} UP0001-${TITLE_ID}_00-0000000000000000 ${PS3DEV_DIR}/bin/ICON0.PNG)
