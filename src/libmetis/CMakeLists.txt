add_library(LibMetis::metis
    STATIC
    IMPORTED
    GLOBAL
)

set_target_properties(LibMetis::metis
    PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libmetis.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
