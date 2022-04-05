cmake_minimum_required(VERSION 3.0)

project(test C)

include($ENV{PS3DEV}/contrib/ppu-helpers.cmake)

add_executable(test_c test.c)
add_ps3_build_steps(test_c)
setup_ps3_pkg_nc(test_c Untitled UNTITLED1 ${PS3DEV_DIR}/bin/sfo.xml UP0001-UNTITLED1_00-0000000000000000 ${PS3DEV_DIR}/bin/ICON0.PNG)
