add_library(entitymanager
    EntityManager.cpp
    EntityManager.h
)

target_link_libraries(entitymanager
    PRIVATE entity
    PRIVATE components
    PRIVATE sfml-graphics
)

target_include_directories(entitymanager
    PUBLIC      ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC     ${CMAKE_CURRENT_SOURCE_DIR}/..
)
