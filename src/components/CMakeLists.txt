add_library(components
    Components.cpp
    Components.h
)

target_link_libraries(components
    PRIVATE vec2
)

target_include_directories(components
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)