cmake_minimum_required(VERSION 3.0)

project(test_cxx CXX)

set(TITLE "Test C++ CMake PSL1GHT")
set(TITLE_ID "UNTITLXX1")
set(VERSION "01.00")
set(LICENSE "MIT LICENSE")

include($ENV{PS3DEV}/contrib/ppu-helpers.cmake)

add_executable(test_cxx test.cpp)
add_ps3_build_steps(test_cxx)
setup_ps3_pkg_nc(test_cxx ${TITLE} ${TITLE_ID} UP0001-${TITLE_ID}_00-0000000000000000 ${PS3DEV_DIR}/bin/ICON0.PNG)
