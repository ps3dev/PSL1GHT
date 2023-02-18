if(NOT $ENV{PS3DEV} STREQUAL "")
    set(PS3DEV_DIR $ENV{PS3DEV})
endif()

if(NOT PS3DEV_DIR)
    message(FATAL_ERROR "Location of PSL1GHT SDK not set")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR powerpc64)
set(TOOLCHAIN_PREFIX ppu)
set(TOOL_OS_SUFFIX "")

if(CMAKE_HOST_WIN32)
    set(TOOL_OS_SUFFIX ".exe")
endif()

set(PS3 1)
set(PSL1GHT 1)

set(CMAKE_C_COMPILER "${PS3DEV_DIR}/ppu/bin/${TOOLCHAIN_PREFIX}-gcc${TOOL_OS_SUFFIX}")
set(CMAKE_C_STANDARD_LIBRARIES "${PS3DEV_DIR}/ppu/ppu/lib/libc.a ${PS3DEV_DIR}/ppu/lib/librt.a ${PS3DEV_DIR}/ppu/lib/liblv2.a")
set(CMAKE_C_LINK_EXECUTABLE "${PS3DEV_DIR}/ppu/bin/${TOOLCHAIN_PREFIX}-gcc${TOOL_OS_SUFFIX} <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o<TARGET> <LINK_LIBRARIES>")

set(CMAKE_CXX_COMPILER "${PS3DEV_DIR}/ppu/bin/${TOOLCHAIN_PREFIX}-g++${TOOL_OS_SUFFIX}")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES} ${PS3DEV_DIR}/ppu/ppu/lib/libstdc++.a")
set(CMAKE_CXX_LINK_EXECUTABLE "${PS3DEV_DIR}/ppu/bin/${TOOLCHAIN_PREFIX}-g++${TOOL_OS_SUFFIX} <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o<TARGET> <LINK_LIBRARIES>")

set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_SUFFIX_C ".a")
set(CMAKE_STATIC_LIBRARY_SUFFIX_CXX ".a")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_SYSROOT "${PS3DEV_DIR}/ppu")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
