#include "Game.h"
#include <iostream>
#include <fstream>
#include <imgui.h> // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include <imgui-SFML.h> // for ImGui::SFML::* functions and SFML-specific overloads


Game::Game(const std::string &config)
    : m_text(m_font) // Initialize sf::Text with font reference - SFML 3 requires this
{
    init(config);
}

void Game::init(const std::string &path)
{
    // TODO: read in config file here
    //       use the pre made PlayerConfig, EnemyConfig, BulletConfig variables

    ssDebug << "starting debug\n";

    // Check if the file exists
    if (!std::filesystem::exists(path))
        ssDebug << path + " File does not exist\n";
    else
    {

    }

        ssDebug << "File " + path + " exists\n";

    if (std::ifstream fin(path); !fin)
        ssDebug << "Could not open config file: " + path + "\n";
    else
    {
        std::string type;
        std::string line;
        ssDebug << "starting debugg\n";

        while (fin >> type)
        {
            // debugOutput += line + "\n";
            std::getline(fin, line);
            // debugOutput += type + " " + line + "\n";
            parseConfig(type, line);
        }
    }

    // set up default window parameters
    m_window.create(sf::VideoMode(sf::Vector2u(m_windowConfig.W, m_windowConfig.H)), "Assignment 2");
    m_window.setFramerateLimit(m_windowConfig.FL);

    // Load a font first
    if (!m_font.openFromFile("assets/" + m_fontConfig.fontFile)) {
        // Try to load a common system font as fallback
        if (!m_font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cerr << "Failed to load fonts!" << std::endl;
        }
    }

    // Configure text after font is loaded
    m_text.setString("Score: 0");
    m_text.setCharacterSize(m_fontConfig.fontSize);

    if (!ImGui::SFML::Init(m_window)) {
        std::cerr << "Failed to initialize ImGui-SFML" << std::endl;
    }

    // scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(1.4f);
    ImGui::GetIO().FontGlobalScale = 1.4f;

    spawnPlayer();
}


std::shared_ptr<Entity> Game::player()
{
    auto& players = m_entities.getEntities("player");
    // assert(players.size() == 1);
    return players.front();
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

    // cleanup
    ImGui::SFML::Shutdown();
    // handle window close
    m_window.close();
}

void Game::setPaused(bool paused) {
    // TODO
}

// respawn the player in the middle of the screen
void Game::spawnPlayer() {
    // get the current player tag and set the player to be removed

    // TODO: Finish adding all properties of the player
    // with the correct values from config

    // We create every entity by calling EntityManager.addEntity(tag)
    // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
    auto entity = m_entities.addEntity("player");

    // Give this entity a Transform, so it spawns at (200,200) with velocity (1,1) and angle 0
    entity->add<CTransform>(Vec2f(200.0f, 200.0f), Vec2f(m_playerConfig.S, m_playerConfig.S), 0.0f);

    // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
    entity->add<CShape>(m_playerConfig.SR, m_playerConfig.V,
            sf::Color(0, 0, 255),
            // sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
            m_playerConfig.OT);

    // Add an input component to the player so that we can use inputs
    entity->add<CInput>();
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
    for (const auto& e: m_entities.getEntities())
    {
        // if entity has transform component...
        if (e->tag() == "player") continue;


        // std::cout << e->tag() << std::endl;
    }

    // Sample movement speed update
    // auto& transform = player()->get<CTransform>();
    // transform.pos.x += transform.velocity.x;


    if (player() && player()->has<CTransform>()) {
        auto& transform = player()->get<CTransform>();
        const auto& input = player()->get<CInput>();

        if (input.up) transform.pos.y -= transform.velocity.y;     // Up decreases Y
        if (input.down) transform.pos.y += transform.velocity.y;   // Down increases Y
        if (input.left) transform.pos.x -= transform.velocity.x;   // Left decreases X
        if (input.right) transform.pos.x += transform.velocity.x;  // Right increases X

        // update rotation
        transform.angle += 1;
        transform.angle = std::fmod(transform.angle, 360.0f);
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
    // add dials for player velocity and size
    if (ImGui::CollapsingHeader("Configure Options"))
    {
        ImGui::BeginGroup();
        {
            ImGui::SeparatorText("Change Velocity");
            static float xVel = 5, yVel = 5;
            ImGui::SliderFloat("Player X Velocity", &xVel,
                    1.0f, 10.0f);
            ImGui::SliderFloat("Player Size", &yVel,
                    1.0f, 10.0f);

            player()->get<CTransform>().velocity = Vec2f(xVel, yVel);
        }
        ImGui::EndGroup();
    }

    if (ImGui::CollapsingHeader("Logging"))
    {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), ssDebug.str().c_str());

    }

    ImGui::End();
}

void Game::sRender() {
    // TODO: change the code below to draw ALL of the entities
    // sample drawing of the player Entity that we have created
    m_window.clear();

    // Get all entities
    auto entities = m_entities.getEntities();
    // Iterate through all entities
    for (const auto& entity : entities) {
        // Only draw entities that have both transform and shape components
        if (entity->has<CTransform>() && entity->has<CShape>()) {
            auto& transform = entity->get<CTransform>();
            auto& shape = entity->get<CShape>();

            // Update position and rotation
            shape.circle.setPosition(transform.pos);
            shape.circle.setRotation(sf::degrees(transform.angle));

            // Draw the entity
            m_window.draw(shape.circle);
        }
    }

    // player()->get<CShape>().circle.setPosition(
    //         player()->get<CTransform>().pos
    //         );
    //
    // // set the rotation of the shape based on the entity's transform->angle
    // player()->get<CTransform>().angle += 1.0f;
    // const auto angle = player()->get<CTransform>().angle;
    // player()->get<CShape>().circle.setRotation(sf::degrees(angle));
    // set the position of the shape based on the entity's transform->pos

    // draw the entity's sf::CircleShape
    // m_window.draw(player()->get<CShape>().circle);

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
            switch (keyEvent->scancode) {
                case sf::Keyboard::Scancode::W:
                    std::cout << "W Key Pressed\n";
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().up = true;
                    }
                    break;
                case sf::Keyboard::Scancode::S:
                    std::cout << "S Key Pressed\n";
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().down = true;
                    }
                    break;
                case sf::Keyboard::Scancode::A:
                    std::cout << "A Key Pressed\n";
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().left = true;
                    }
                    break;
                case sf::Keyboard::Scancode::D:
                    std::cout << "D Key Pressed\n";
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().right = true;
                    }
                    break;
                case sf::Keyboard::Scancode::Escape:
                    std::cout << "ESC Key Pressed - Ending Game\n";
                    m_running = false;
                    break;
                default:
                    // Handle any other keys or do nothing
                    break;
            }
        }
        else if (const auto* keyEvent = event.getIf<sf::Event::KeyReleased>()) {
            switch (keyEvent->scancode) {
                case sf::Keyboard::Scancode::W:
                    std::cout << "W Key Released\n";
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().up = false;
                    }
                    break;
                case sf::Keyboard::Scancode::S:
                    std::cout << "S Key Released\n";
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().down = false;
                    }
                    break;
                case sf::Keyboard::Scancode::A:
                    std::cout << "A Key Released\n";
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().left = false;
                    }
                    break;
                case sf::Keyboard::Scancode::D:
                    std::cout << "D Key Released\n";
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().right = false;
                    }
                    break;
                default:
                    // Handle any other keys or do nothing
                    break;
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

void Game::parseConfig(const std::string & type, const std::string & values)
{
    // TODO: parse the values string and set the variables accordingly
    // Window 1280 720 60 0
    // Font fonts/Ribeye-Regular.ttf 24 255 255 255
    // Player 32 32 5 5 5 5 255 0 0 4 8 500
    // Enemy 32 32 3 3 255 255 255 2 3 8 90 60
    // Bullet 10 10 20 255 255 255 255 255 255 2 20 3
    // // W = width, H = height, FL = frame limit, FS = full screen ( 1 = true, 0 = false)

    std::stringstream ss(values);

    try
    {
        std::string value;
        if (type == "Window")
        {
            ss >> value;
            m_windowConfig.W = std::stoi(value);
            ss >> value;
            m_windowConfig.H = std::stoi(value);
            ss >> value;
            m_windowConfig.FL = std::stoi(value);
            ss >> value;
            m_windowConfig.FS = std::stoi(value);
            // struct WindowConfig{int W, H, FL, FS;};
            return;
        }

        if (type == "Player")
        {
            // struct PlayerConfig{int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
            // shape radius
            ss >> value;
            m_playerConfig.SR = std::stoi(value); // collision radius
            ss >> value;
            m_playerConfig.CR = std::stoi(value);
            // speed
            ss >> value;
            m_playerConfig.S = std::stof(value);
            // fill color
            ss >> value;
            m_playerConfig.FR = std::stoi(value);

            ss >> value;
            m_playerConfig.FG = std::stoi(value);

            ss >> value;
            m_playerConfig.FB = std::stoi(value);

            // outline color
            ss >> value;
            m_playerConfig.OR = std::stoi(value);

            ss >> value;
            m_playerConfig.OG = std::stoi(value);

            ss >> value;
            m_playerConfig.OB = std::stoi(value);
            // outline thickness
            ss >> value;
            m_playerConfig.OT = std::stoi(value);
            // shape vertices
            ss >> value;
            m_playerConfig.V = std::stoi(value);
            return;
        }

        if (type == "Enemy")
        {
            // struct EnemyConfig {int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX;};
            ss >> value;
            m_enemyConfig.SR = std::stoi(value);

            ss >> value;
            m_enemyConfig.CR = std::stoi(value);

            ss >> value;
            m_enemyConfig.SMIN = std::stof(value);

            ss >> value;
            m_enemyConfig.SMAX = std::stof(value);

            ss >> value;
            m_enemyConfig.OR = std::stoi(value);
            ss >> value;
            m_enemyConfig.OG = std::stoi(value);
            ss >> value;
            m_enemyConfig.OB = std::stoi(value);
            ss >> value;
            m_enemyConfig.OT = std::stoi(value);
            ss >> value;
            m_enemyConfig.VMIN = std::stoi(value);
            ss >> value;
            m_enemyConfig.VMAX = std::stoi(value);
            ss >> value;
            m_enemyConfig.L = std::stoi(value);
            ss >> value;
            m_enemyConfig.SI = std::stoi(value);
            return;
        }

        if (type == "Bullet")
        {
            // struct BulletConfig{int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };
            ss >> value;
            m_bulletConfig.SR = std::stoi(value);
            ss >> value;
            m_bulletConfig.CR = std::stoi(value);
            ss >> value;
            m_bulletConfig.S = std::stof(value);
            ss >> value;
            m_bulletConfig.FR = std::stoi(value);
            ss >> value;
            m_bulletConfig.FG = std::stoi(value);
            ss >> value;
            m_bulletConfig.FB = std::stoi(value);
            ss >> value;
            m_bulletConfig.OR = std::stoi(value);
            ss >> value;
            m_bulletConfig.OG = std::stoi(value);
            ss >> value;
            m_bulletConfig.OB = std::stoi(value);
            ss >> value;
            m_bulletConfig.OT = std::stoi(value);
            ss >> value;
            m_bulletConfig.V = std::stoi(value);
            ss >> value;
            m_bulletConfig.L = std::stoi(value);
            return;
        }

        if (type == "Font")
        {
            // struct FontConfig{std::string fontFile; int fontSize; int R, G, B;};
            ss >> value;
            m_fontConfig.fontFile = value;
            ss >> value;
            m_fontConfig.fontSize = std::stoi(value);
            ss >> value;
            m_fontConfig.R = std::stoi(value);
            ss >> value;
            m_fontConfig.G = std::stoi(value);
            ss >> value;
            m_fontConfig.B = std::stoi(value);
            return;
        }
    }
    catch (std::exception& e)
    {
        ssDebug << "Error parsing " << type << ": " << e.what() << "\n";
        ssDebug << "Values: '" << values << "'\n";
    }
}
