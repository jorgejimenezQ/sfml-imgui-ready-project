# Define the shapes library
add_library(shapes 
    Shape.cpp 
    Shape.h
)

# Link with necessary libraries
target_link_libraries(shapes
    PUBLIC sfml-graphics
)

# Include directories
target_include_directories(shapes
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)