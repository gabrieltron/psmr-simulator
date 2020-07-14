add_library(request)

target_sources(
    request
        PUBLIC
            random.h
            request_generation.h
        PRIVATE
            random.cpp
            request_generation.cpp
)

target_include_directories(
    request
        PUBLIC
            "${CMAKE_SOURCE_DIR}/src"
)
