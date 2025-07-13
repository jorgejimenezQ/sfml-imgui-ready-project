# Add subdirectories
add_subdirectory(shapes)
add_subdirectory(vec2)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(entity)
add_subdirectory(entitymanager)
add_subdirectory(game)

# Main executable
add_executable(CMakeLearn main.cpp)

# Link with dependencies
# The PRIVATE keyword specifies that the libraries are only used by the
# CMakeLearn target and are not propagated to its dependents.
#
# ImGui-SFML is a dependency of CMakeLearn, which means that any target that
# links to CMakeLearn will also link to ImGui-SFML.
#
# The shapes library is also a dependency of CMakeLearn, and is linked to
# privately, which means that it is not propagated to targets that link to
# CMakeLearn.
target_link_libraries(CMakeLearn
        PRIVATE ImGui-SFML::ImGui-SFML
        PRIVATE shapes
        PRIVATE vec2
        PRIVATE components
        PRIVATE systems
        PRIVATE entity
        PRIVATE entitymanager
        PRIVATE game
)

# Apply the same compiler warnings as ImGui-SFML
#
# The PRIVATE keyword specifies that the compiler options are only applied to
# the CMakeLearn target and are not propagated to its dependents.
target_compile_options(CMakeLearn PRIVATE ${IMGUI_SFML_WARNINGS})