add_library(manager)

target_sources(
    manager
        PUBLIC
            manager.h
            cbase_manager.h
            early_min_cut_manager.h
            graph_cut_manager.h
            min_cut_manager.h
            tree_cut_manager.h
        PRIVATE
            manager.cpp
            cbase_manager.cpp
            early_min_cut_manager.cpp
            graph_cut_manager.cpp
            min_cut_manager.cpp
            tree_cut_manager.cpp
)

target_include_directories(
    manager
        PUBLIC
            "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(
    manager
        PUBLIC
            CONAN_PKG::toml11
            graph
            log
            partition
            request
)
