cmake_minimum_required(VERSION 3.28)
project(
    CMakeLearn
    VERSION 1.0
    DESCRIPTION "CMakeLearn"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(./dependencies)
add_subdirectory(./src)


