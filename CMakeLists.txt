cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Simulator VERSION 1.0)

add_executable(
    main
)

target_sources(
    main
        PRIVATE
            src/cbase_manager.cpp
            src/early_min_cut_manager.cpp
            src/execution_log.cpp
            src/graph_cut_manager.cpp
            src/graph.cpp
            src/main.cpp
            src/manager.cpp
            src/min_cut_manager.cpp
            src/min_cut.cpp
            src/node.cpp
            src/partition_scheme.cpp
            src/partition.cpp
            src/random.cpp
            src/request_generation.cpp
            src/spanning_tree.cpp
            src/tree_cut_manager.cpp
            src/write.cpp
)

target_include_directories(
    main
        PRIVATE
            include
            libs
)

add_subdirectory(src/libkahip)

target_compile_features(
    main
        PRIVATE
            cxx_std_17
)

target_link_libraries(
    main
        PRIVATE
            metis
            LibKahip::kahip
)
