add_library(vec2
    Vec2.cpp
    Vec2.h)

target_link_libraries(vec2
    sfml-graphics
)

target_include_directories(vec2
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)