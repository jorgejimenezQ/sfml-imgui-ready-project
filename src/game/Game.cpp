#include "Game.h"
#include <iostream>
#include <imgui.h> // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include <imgui-SFML.h> // for ImGui::SFML::* functions and SFML-specific overloads


Game::Game(const std::string &config)
    : m_text(m_font) // Initialize sf::Text with font reference - SFML 3 requires this
{
    init(config);
}

void Game::init(const std::string &path) {
    // TODO: read in config file here
    //       use the pre made PlayerConfig, EnemyConfig, BulletConfig variables

    // set up default window parameters
    m_window.create(sf::VideoMode(sf::Vector2u(1280, 720)), "Assignment 2");
    m_window.setFramerateLimit(60);

    // Load a font first
    if (!m_font.openFromFile("arial.ttf")) {
        // Try to load a common system font as fallback
        if (!m_font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cerr << "Failed to load fonts!" << std::endl;
        }
    }
    
    // Configure text after font is loaded
    m_text.setString("Score: 0");
    m_text.setCharacterSize(30);

    if (!ImGui::SFML::Init(m_window)) {
        std::cerr << "Failed to initialize ImGui-SFML" << std::endl;
    }

    // scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    spawnPlayer();
}

void Game::run() {
    // TODO:
    // - add pause functionality in here
    // - some systems should function while paused (rendering)
    // - some systems shouldn't (movement / input)
    while (m_running)
    {
        // update the entity manager
        m_entities.update();

        // required update call to imgui
        ImGui::SFML::Update(m_window, m_deltaClock.restart());

        sEnemySpawner();
        sMovement();
        sCollision();
        sUserInput();
        sGUI();
        sRender();

        // increment the current frame
        // may need to be moved when pause implemented
        m_currentFrame++;
    }
}

void Game::setPaused(bool paused) {
    // TODO
}

// respawn the player in the middle of the screen
void Game::spawnPlayer() {
    // TODO: Finish adding all properties of the player with the correct values from config

    // We create every entity by calling EntityManager.addEntity(tag)
    // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
    auto entity = m_entities.addEntity("player");

    // Give this entity a Transform, so it spawns at (200,200) with velocity (1,1) and angle 0
    entity->add<CTransform>(Vec2f(200.0f, 200.0f), Vec2f(1.0f, 1.0f), 0.0f);

    // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
    entity->add<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

    // Add an input component to the player so that we can use inputs
    entity->add<CInput>();

    // Since we want this Entity to be our player, set our Game's player variable to be this Entity
    // This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
    m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy() {
    // TODO:
    // make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy must be spawned completely within the bounds of the window

    // auto e = m_entities.addEntity("enemy");
    // e->add<CTransform>(args);
    // e->add<CShape>(args);

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e) {
    // TODO: spawn small enemies at the location of the input enemy e

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to the vertices of the original enemy
    // - set each small enemy to the same color as the original, half the size
    // - small enemies are worth double points of the original enemy
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &target) {
    // TODO: implement the spawning of a bullet which travels toward target
    // - bullet speed is given as a scalar speed
    // - you must set the velocity by using formula in notes
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {
    // TODO: implement your own special weapon
}

void Game::sMovement() {
    // TODO: implement all entity movement in this function
    // you should read the m_player->cInput component to determine if the player is moving
    // sample:
    for (auto e: m_entities.getEntities())
    {
        // if entity has transform component...
    }

    // Sample movement speed update
    if (m_player && m_player->has<CTransform>()) {
        auto& transform = m_player->get<CTransform>();
        transform.pos.x += transform.velocity.x;
        transform.pos.y += transform.velocity.y;
    }
}

void Game::sLifespan() {
    // TODO: implement all lifespan functionality
    //
    // for all entities
    // - if entity has no lifespan component, skip it
    // - if entity has > 0 remaining lifespan, subtract 1
    // - if it has lifespan and is alive scale its alpha channel properly
    // - if it has lifespan and its time is up destroy the entity
}

void Game::sCollision() {
    // TODO: implement all proper collisions between entities
    // be sure to use the collision radius, not the shape radius
    // sample
//    for (auto bullet: m_entities.getEntities("bullet")) {
//        for (auto m_entities: m_entities.getEntities()) {
//            //check collision
//        }
//        // we need another loop for small entities because small entities don't spawn further
//    }
}

void Game::sEnemySpawner() {
    // TODO: code which implements enemy spawning should go here
}

void Game::sGUI() {
    ImGui::Begin("Geometry Wars");

    ImGui::Text("Stuff Goes Here");

    ImGui::End();
}

void Game::sRender() {
    // TODO: change the code below to draw ALL of the entities
    // sample drawing of the player Entity that we have created
    m_window.clear();

    if (m_player && m_player->has<CShape>() && m_player->has<CTransform>()) {
        auto& shape = m_player->get<CShape>();
        auto& transform = m_player->get<CTransform>();
        
        // set the position of the shape based on the entity's transform->pos
        shape.circle.setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));

        // set the rotation of the shape based on the entity's transform->angle
        transform.angle += 1.0f;
        // Use the sf::degrees function to convert float to sf::Angle
        shape.circle.setRotation(sf::degrees(transform.angle));

        // draw the entity's sf::CircleShape
        m_window.draw(shape.circle);
    }

    // draw the ui last
    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::sUserInput() {
    // TODO: handle user input here
    // note that you should only be setting the player's input component variables here
    // you should not implement the player's movement logic here
    // the movement system will read the variables you set in this function

    // Handle events using SFML 3.0 API with std::optional
    std::optional<sf::Event> optEvent = m_window.pollEvent();
    
    while (optEvent) {
        const auto& event = *optEvent;
        
        // pass the event to imgui to be parsed
        ImGui::SFML::ProcessEvent(m_window, event);

        // Using the SFML 3.0 event handling API with is<T> and getIf<T> methods
        if (event.is<sf::Event::Closed>()) {
            m_running = false;
        }
        else if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->scancode == sf::Keyboard::Scancode::W) {
                std::cout << "W Key Pressed\n";
                if (m_player && m_player->has<CInput>()) {
                    m_player->get<CInput>().up = true;
                }
            }
        }
        else if (const auto* keyEvent = event.getIf<sf::Event::KeyReleased>()) {
            if (keyEvent->scancode == sf::Keyboard::Scancode::W) {
                std::cout << "W Key Released\n";
                if (m_player && m_player->has<CInput>()) {
                    m_player->get<CInput>().up = false;
                }
            }
        }
        else if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
            // this line ignores mouse events if ImGui is the thing being clicked
            if (ImGui::GetIO().WantCaptureMouse) {
                optEvent = m_window.pollEvent();
                continue;
            }

            if (mouseEvent->button == sf::Mouse::Button::Left) {
                std::cout << "Left Mouse Button Clicked at(" << mouseEvent->position.x
                          << ", " << mouseEvent->position.y << ")\n";
                // TODO: call spawnBullet here
            }
            else if (mouseEvent->button == sf::Mouse::Button::Right) {
                std::cout << "Right Mouse Button Clicked at(" << mouseEvent->position.x
                          << ", " << mouseEvent->position.y << ")\n";
                // TODO: call special weapon here
            }
        }
        
        optEvent = m_window.pollEvent();
    }
}
