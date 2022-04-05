cmake_minimum_required(VERSION 3.0)

project(test_cxx CXX)

include($ENV{PS3DEV}/contrib/psl1ght.cmake)

add_executable(test_cxx test.cpp)
add_ps3_build_steps(test_cxx)
setup_ps3_pkg_nc(test_cxx "Untitled C++" UNTITLXX1 ${PS3DEV_DIR}/bin/sfo.xml UP0001-UNTITLXX1_00-0000000000000000 ${PS3DEV_DIR}/bin/ICON0.PNG)
