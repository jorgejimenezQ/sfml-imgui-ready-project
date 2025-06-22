add_library(entity
    Entity.cpp
    Entity.h
)

target_link_libraries(entity
    PRIVATE components
    PRIVATE sfml-graphics
)

target_include_directories(entity
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
