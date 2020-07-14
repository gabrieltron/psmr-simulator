add_subdirectory(graph)
add_subdirectory(log)
add_subdirectory(manager)
add_subdirectory(partition)
add_subdirectory(request)
add_subdirectory(write)
add_subdirectory(libkahip)
add_subdirectory(libmetis)

add_executable(main)

target_sources(
    main
        PRIVATE
            main.cpp
)

target_link_libraries(
    main
        PRIVATE
            CONAN_PKG::toml11
            manager
            log
            partition
            request
            write
)
