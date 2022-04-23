cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(RemoveChannels)

cmake_policy(SET CMP0074 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(Nuke REQUIRED)

message("Using Nuke ${NUKE_VERSION_MAJOR}.${NUKE_VERSION_MINOR}v${NUKE_VERSION_RELEASE}")

if (UNIX)
    add_compile_options(
        -DUSE_GLEW -fPIC -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx
    )
endif()

set(CMAKE_CXX_STANDARD 14)

# add sub directory
add_subdirectory(src)