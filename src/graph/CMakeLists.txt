add_library(graph)

target_sources(
    graph
        PUBLIC
            graph.h
            node.h
            spanning_tree.h
        PRIVATE
            graph.cpp
            node.cpp
            spanning_tree.cpp
)

target_include_directories(
    graph
        PUBLIC
            "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(
    graph
        PUBLIC
            partition
)
