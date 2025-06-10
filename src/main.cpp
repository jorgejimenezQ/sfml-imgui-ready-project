#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads
#include <SFML/Graphics.hpp>
#include "shapes/Shape.h"

#include <iostream>

static void showWindowWithMenu()
{
    ImGui::Begin("Window");
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Open..", "Ctrl+O");
                ImGui::MenuItem("Save", "Ctrl+S");
                ImGui::MenuItem("Close", "Ctrl+W");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("Hello, world!");
    ImGui::End();
}

static void showWindowWithcCheckbox(const char* label, bool* v)
{
    ImGui::Begin("Checkbox");
        ImGui::LabelText("label", "%s", label);
        ImGui::Checkbox(label, v);
    ImGui::End();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({1140, 680}), "ImGui + SFML = <3");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        std::cerr << "ImGui SFML initialization failed" << std::endl;
        return 1;
    }
    
    // Use our new Shape class instead of sf::CircleShape directly
    cmakelearn::Shape shape;
    shape.setPosition(570.f, 340.f); // center in window
    static bool check = true;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        showWindowWithMenu();
        showWindowWithcCheckbox("Checkbox", &check);
        window.clear();
        if (check)
        {
            shape.setFillColor(sf::Color::Green);
        }
        else
        {
            shape.setFillColor(sf::Color::Red);
        }
        
        // Use our draw method instead
        shape.draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
