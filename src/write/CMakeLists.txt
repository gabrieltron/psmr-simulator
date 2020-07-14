add_library(write)

target_sources(
    write
        PUBLIC
            write.h
        PRIVATE
            write.cpp
)

target_include_directories(
    write
        PUBLIC
            "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(
    write
        PUBLIC
            log
            graph
            partition
)
