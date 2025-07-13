cmake_minimum_required(VERSION 3.28)
project(
    CMakeLearn
    VERSION 1.0
    DESCRIPTION "CMakeLearn"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

# Copy assets to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/src)

# Output the build directory and the path to the assets
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

add_subdirectory(./dependencies)
add_subdirectory(./src)
