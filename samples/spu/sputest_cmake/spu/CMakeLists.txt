cmake_minimum_required(VERSION 3.0)

project(test_spu C)

include($ENV{PS3DEV}/contrib/spu-helpers.cmake)

add_executable(test_spu test.c)
add_spu_dirs(test_spu)
