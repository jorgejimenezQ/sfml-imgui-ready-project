add_library(systems
        Systems.cpp
        Systems.h
)

target_link_libraries(systems
        PRIVATE vec2
)

target_include_directories(systems
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)