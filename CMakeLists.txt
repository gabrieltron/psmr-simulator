cmake_minimum_required(VERSION 3.15)
project(smr-simulator CXX)

include(cmake/base.cmake)
include(cmake/project_options.cmake)
include(cmake/conan.cmake)

conan(
    PACKAGES
        toml11/3.4.0
)

include(cmake/add_libkahip.cmake)
include(cmake/add_libmetis.cmake)

add_subdirectory(src)
