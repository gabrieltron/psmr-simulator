add_library(partition)

target_sources(
    partition
        PUBLIC
            min_cut.h
            partition.h
            partition_scheme.h
        PRIVATE
            min_cut.cpp
            partition.cpp
            partition_scheme.cpp
)

target_include_directories(
    partition
        PUBLIC
            "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(
    partition
        PUBLIC
            LibMetis::metis
            LibKahip::kahip
            graph
)
