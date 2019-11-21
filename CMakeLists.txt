cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Simulator VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Bring the headers into the project
include_directories(include libs)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})
target_link_libraries(main metis)
