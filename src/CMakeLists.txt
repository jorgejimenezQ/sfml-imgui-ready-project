add_executable(CMakeLearn main.cpp
        Shape.cpp
        Shape.h)


target_link_libraries(CMakeLearn PRIVATE ImGui-SFML::ImGui-SFML)
target_compile_options(CMakeLearn PRIVATE ${IMGUI_SFML_WARNINGS})