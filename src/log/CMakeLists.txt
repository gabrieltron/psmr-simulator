add_library(log)

target_sources(
    log
        PUBLIC
            execution_log.h
        PRIVATE
            execution_log.cpp
)
