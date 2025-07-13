add_library(game
        Game.cpp
        Game.h
)

target_link_libraries(game
        PRIVATE entitymanager
        PRIVATE systems
        PRIVATE sfml-graphics
        PRIVATE vec2
        PRIVATE ImGui-SFML
)

target_include_directories(game
        PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC     ${CMAKE_CURRENT_SOURCE_DIR}/..
        PRIVATE     ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)