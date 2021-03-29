cmake_minimum_required(VERSION 3.20)
project(PSL1GHT_CMake)

set(CMAKE_CXX_STANDARD 17)

include(cmake/psl1ght.cmake)

add_executable(PSL1GHT_CMake source/main.c)
setupPSL1GHT(PSL1GHT_CMake)
setupPSL1GHTpkg(PSL1GHT_CMake Untitiled UNTITLED1 ${PSL1GHT}/bin/sfo.xml UP0001-UNTITLED1_00-0000000000000000 ${PSL1GHT}/bin/ICON0.PNG)
