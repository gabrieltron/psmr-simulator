add_library(LibKahip::kahip
    STATIC
    IMPORTED
    GLOBAL
)

set_target_properties(LibKahip::kahip
    PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libfodase.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
