#include "Game.h"
#include <iostream>
#include <fstream>
#include <imgui.h> // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include <imgui-SFML.h> // for ImGui::SFML::* functions and SFML-specific overloads
#include <cstdlib>
#include <random>
#include <numbers>

Game::Game(const std::string &config)
    : m_text(m_font) // Initialize sf::Text with font reference - SFML 3 requires this
{
    init(config);
}

void Game::init(const std::string &config)
{
    srand(time(nullptr));

    ssDebug << "starting debug output:\n\n";

    // Check if the file exists
    if (!std::filesystem::exists(config))
        ssDebug << config + " File does not exist\n";
    else
    {
        ssDebug << "File " + config + " exists\n";
    }


    if (std::ifstream fin(config); !fin)
        ssDebug << "Could not open config file: " + config + "\n";
    else
    {
        std::string type;
        std::string line;

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
    m_text.setFont(m_font);
    m_text.setCharacterSize(m_fontConfig.fontSize);

    if (!ImGui::SFML::Init(m_window)) {
        std::cerr << "Failed to initialize ImGui-SFML" << std::endl;
    }

    // scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(1.2f);
    ImGui::GetIO().FontGlobalScale = 1.2f;

    spawnPlayer();
}


std::shared_ptr<Entity> Game::player()
{
    auto& players = m_entities.getEntities("player");
    // assert(players.size() == 1);
    return players.front();
}


void Game::run() {
    // - add pause functionality in here
    // - some systems should function while paused (rendering)
    // - some systems shouldn't (movement / input)
    while (m_running)
    {
        // update the entity manager
        m_entities.update();

        // required update call to imgui
        ImGui::SFML::Update(m_window, m_deltaClock.restart());

        if (!m_paused)
        {

            if (!m_isEnemeySpawnDisabled) sEnemySpawner();
            if(!m_isMovementDisabled) sMovement();
            if (!m_isCollisionDisabled) sCollision();
            if(!m_isLifespanDisabled) sLifespan();
        }

        sUserInput();
        sGUI();
        sRender();

        // increment the current frame
        // may need to be moved when pause implemented
        if (!m_paused) m_currentFrame++;
    }

    // cleanup
    ImGui::SFML::Shutdown();
    // handle window close
    m_window.close();
}

void Game::setPaused(const bool paused) {
    m_paused = paused;
}

// respawn the player in the middle of the screen
void Game::spawnPlayer() {
    // Create the base entity with common components
    auto entity = createEntity(
        "player",                           // tag
        Vec2f(m_windowConfig.W / 2.0f, m_windowConfig.H / 2.0f),  // position
        m_playerConfig.SR,                  // shape radius
        m_playerConfig.V,                   // vertex count
        sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),  // fill color
        sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),  // outline color
        static_cast<float>(m_playerConfig.OT),  // outline thickness
        Vec2f(m_playerConfig.S, m_playerConfig.S),  // velocity
        0,                                  // lifespan (0 for player)
        m_playerConfig.CR                   // collision radius
    );

    // Add player-specific components
    entity->add<CInput>();

    // Store player entity
    m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy(const std::string& type) {
    // Calculate spawn position, ensuring the enemy is fully within window bounds
    // by accounting for the enemy radius
    const auto enemyRadius = m_enemyConfig.SR;
    
    // Limit spawn area to be within window bounds considering the radius
    const auto minX = enemyRadius;
    const auto maxX = m_windowConfig.W - enemyRadius;
    const auto minY = enemyRadius;
    const auto maxY = m_windowConfig.H - enemyRadius;
    
    // Generate random position within the safe boundaries
    const auto x = static_cast<float>(minX) + (static_cast<float>(rand()) / RAND_MAX) * static_cast<float>(maxX - minX);
    const auto y = static_cast<float>(minY) + (static_cast<float>(rand()) / RAND_MAX) * static_cast<float>(maxY - minY);

    // Vec2f position(m_windowConfig.W / 2 , m_windowConfig.H / 2);
    Vec2f position(x, y);

    // Generate random velocity between VMIN and VMAX
    float speed = m_enemyConfig.SMIN +
              (static_cast<float>(rand()) / RAND_MAX) *
              (m_enemyConfig.SMAX - m_enemyConfig.SMIN);

    // Generate random angle in radians (0 to 2π)
    const auto randomAngle =
      static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;

    // Generate random number of points for the shape
    size_t numPoints = m_enemyConfig.VMIN + (rand() % (1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN));
    
    // Generate random fill color
    uint8_t r = rand() % 255, g = rand() % 255, b = rand() % 255;
    
    // Create the base entity with common components
    auto const e = createEntity(
        type,                           // tag
        position,  // position
        enemyRadius,                  // shape radius
        numPoints,                   // vertex count
        sf::Color(r, g, b),  // fill color
        sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),  // outline color
        static_cast<float>(m_enemyConfig.OT),  // outline thickness
        Vec2f(std::cosf(randomAngle) * speed, std::sinf(randomAngle) * speed),  // velocity
        (type == "spazbit") ? 2000 : m_enemyConfig.L,                                  // lifespan (0 for player)
        m_enemyConfig.CR,                   // collision radius
        static_cast<int>(numPoints*100),
        EASEINOUT_EXPO
    );

    if (type == "spazbit")
    {
        e->add<CSpazJump>();
        e->get<CLifespan>().setEasingType(EASEIN_EXPO);
    }

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> const& e) {

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to the vertices of the original enemy
    // - set each small enemy to the same color as the original, half the size
    // - small enemies are worth double points of the original enemy
    const size_t numEnemies = e->get<CShape>().getPointCount();
    const auto interval = 360/static_cast<float>(numEnemies);
    for (int i = 0; i < numEnemies; i++)
    {
        auto const smallEnemy = m_entities.addEntity("Small Enemy");
        auto& transform = e->get<CTransform>();
        auto& shape = e->get<CShape>();
        const auto rads = (static_cast<float>(i) * interval) * static_cast<float>(M_PI /180.0f);

        smallEnemy->add<CTransform>(
            transform.pos,
            Vec2f(std::cosf(rads), std::sinf(rads)),
            0.0f);

        smallEnemy->add<CShape>(
            shape.getRadius()/2, shape.getPointCount(), // radius, points
            shape.getFillColor(), // fill color
            shape.getOutlineColor(), // outline color
            m_enemyConfig.OT // outline thickness
        );

        smallEnemy->add<CLifespan>(m_enemyConfig.L);
        smallEnemy->get<CLifespan>().setEasingType(EASEIN_EXPO);
        smallEnemy->add<CCollision>(m_enemyConfig.CR);
        smallEnemy->add<CScore>(numEnemies*2*100);
    }
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(const Vec2f &target) {
    auto const playerPos(player()->get<CTransform>().pos);
    auto const diff = target - playerPos;
    auto const normalizedVector = Vec2f::normalize(diff);
    auto const velocity = normalizedVector * m_bulletConfig.S;

    // Create bullet entity with factory
    auto bullet = createEntity(
        "bullet",                           // tag
        playerPos,                          // position
        m_bulletConfig.SR,                  // shape radius
        m_bulletConfig.V,                   // vertex count
        sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),  // fill color
        sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),  // outline color
        static_cast<float>(m_bulletConfig.OT),  // outline thickness
        velocity,                           // velocity
        m_bulletConfig.L,                   // lifespan
        m_bulletConfig.CR,                  // collision radius
        0,                                  // score (bullets don't have score)
        EASEOUT_SINE                        // easing function
    );
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {
    // TODO: implement your own special weapon
}

void Game::spazbitMovement(std::shared_ptr<Entity> entity)
{
    auto& spaz      = entity->get<CSpazJump>();
    auto& transform = entity->get<CTransform>();
    const float r   = entity->get<CShape>().getRadius();
    const auto W   = static_cast<float>(m_windowConfig.W);
    const auto H   = static_cast<float>(m_windowConfig.H);

    // 1) Reset jump if completed (use >= to avoid float-eq)
    if (spaz.distanceTraveled >= spaz.distanceToTravel)
    {
        // 2) Better RNG
        static thread_local std::mt19937 gen{ std::random_device{}() };
        std::uniform_real_distribution angDist(0.0f, 2.0f * std::numbers::pi_v<float>);
        std::uniform_real_distribution speedDist(m_enemyConfig.SMIN, m_enemyConfig.SMAX);

        // random distance to travel
        std::uniform_real_distribution distDist(50.0f, 200.0f);

        const float angle = angDist(gen);
        const float speed = speedDist(gen);

        transform.velocity = Vec2f{ std::cosf(angle) * speed,
                                    std::sinf(angle) * speed };

        spaz.distanceToTravel = distDist(gen);
        spaz.distanceTraveled = 0.0f;
    }

    // 3) Safe progress compute
    float progress = (spaz.distanceToTravel > 0.0f)
        ? std::clamp(spaz.distanceTraveled / spaz.distanceToTravel, 0.0f, 1.0f)
        : 1.0f;

    const float offsetFactor = m_interpolations.interpolate(progress, entity->get<CLifespan>().getEasing());
    const Vec2f movement = transform.velocity * offsetFactor;

    // 4) Move & bounce
    transform.pos += movement;

    // Bounce on X
    if (transform.pos.x - r < 0.0f || transform.pos.x + r > W)
    {
        transform.velocity.x = -transform.velocity.x;
        // clamp so we don’t get stuck outside
        transform.pos.x = std::clamp(transform.pos.x, r, W - r);
    }

    // Bounce on Y
    if (transform.pos.y - r < 0.0f || transform.pos.y + r > H)
    {
        transform.velocity.y = -transform.velocity.y;
        transform.pos.y = std::clamp(transform.pos.y, r, H - r);
    }

    spaz.distanceTraveled += 1.0f;

}

void Game::sMovement() {
    for (const auto& e: m_entities.getEntities())
    {
        // if entity has transform component...
        if (e->tag() == "player") continue;

        if (e->tag() == "spazbit")
        {
           spazbitMovement(e);
            continue;
        }


        if (e->has<CTransform>() && e->has<CShape>())
        {
            auto& transform = e->get<CTransform>();
            const auto& shape = e->get<CShape>();

            const float posX = transform.pos.x;
            const float posY = transform.pos.y;
            const float radius = shape.circle.getRadius();
            const auto windowWidth = static_cast<float>(m_windowConfig.W);
            const auto windowHeight = static_cast<float>(m_windowConfig.H);

            if ((posX - radius) < 0 || (posX + radius) > windowWidth)
                    transform.velocity.x *= -1;
            if ((posY - radius) < 0 || (posY + radius) > windowHeight)
                    transform.velocity.y *= -1;

            transform.pos.y += transform.velocity.y;
            transform.pos.x += transform.velocity.x;
        }
    }

    if (player() && player()->has<CTransform>()) {
        auto& transform = player()->get<CTransform>();
        const auto& input = player()->get<CInput>();

        if (input.up) transform.pos.y -= transform.velocity.y;     // Up decreases Y
        if (input.down) transform.pos.y += transform.velocity.y;   // Down increases Y
        if (input.left) transform.pos.x -= transform.velocity.x;   // Left decreases X
        if (input.right) transform.pos.x += transform.velocity.x;  // Right increases X
    }
}

void Game::sLifespan() {
    // for all entities
    for (const auto& e: m_entities.getEntities())
    {
        if (e->tag() == "spazbit") continue;

        // - if entity has no lifespan component, skip it
        if (!e->has<CLifespan>() || !e->isActive()) continue;

        // - if entity has > 0 remaining lifespan, subtract 1
        auto& lifespan = e->get<CLifespan>();
        lifespan.remaining--;

        if (lifespan.remaining <= 0)
        {
            // - if it has lifespan and its time is up destroy the entity
            e->destroy();
            continue;
        }
            // - if it has lifespan and is alive scale its alpha channel properly
            auto& shape = e->get<CShape>();
            sf::Color currColor = shape.getFillColor();
            
            // Calculate the normalized progress (0.0 to 1.0)
            float progress = static_cast<float>(lifespan.remaining) / static_cast<float>(lifespan.lifespan);
            
            // Clamp progress between 0.1 and 1.0 as requested
            progress = std::max(0.0f, std::min(progress, 1.0f));
            
            // Apply the smooth interpolation to calculate alpha
            const float newAlpha =
                m_interpolations.interpolate(progress, lifespan.getEasing()) * 255.0f;

            shape.setFillColor(sf::Color(currColor.r, currColor.g, currColor.b, static_cast<uint8_t>(newAlpha)));
            shape.setOutlineColor(sf::Color(255, 255, 255, static_cast<uint8_t>(newAlpha)));
    }
}

void Game::sCollision() {
    // TODO: implement all proper collisions between entities
    // be sure to use the collision radius, not the shape radius
    // sample
    for (auto const& bullet: m_entities.getEntities("bullet")) {
        if (!bullet->isActive()) continue;
        const auto& bulletTransform = bullet->get<CTransform>();

        for (auto const& entity: m_entities.getEntities())
        {
            if (entity->tag() == "bullet" || entity->tag() == "player") continue;
            if (!entity->isActive()) continue;

            //check collision
            const auto& entityTransform = entity->get<CTransform>();
            const auto diff = bulletTransform.pos - entityTransform.pos;
            const auto dist = diff.x*diff.x + diff.y*diff.y;
            const auto r1 = bullet->get<CCollision>().radius;
            const auto r2 = entity->get<CCollision>().radius;

            if (dist < ((r1+r2) * (r1+r2)))
            {
                if (entity->tag() == "enemy") spawnSmallEnemies(entity);
                m_score += entity->get<CScore>().score;
                bullet->destroy();
                entity->destroy();

            }
        }
        //
        // for (auto const& entity: m_entities.getEntities())
        // {
        //     if (entity->tag() == "butllet" || entity->tag() == "player") continue;
        //     if (!entity->isActive()) continue;
        //     //check collision
        //     const auto entityTransform = entity->get<CTransform>();
        //     const auto diff = bulletTransform.pos - entityTransform.pos;
        //     const auto dist = diff.x*diff.x + diff.y*diff.y;
        //     const auto r1 = bullet->get<CCollision>().radius;
        //     const auto r2 = entity->get<CCollision>().radius;
        //
        //     if (dist < ((r1+r2) * (r1+r2)) && entity->tag() == "enemy")
        //     {
        //         // spawnSmallEnemies(entity);
        //         bullet->destroy();
        //         entity->destroy();
        //     }
        // }
        // we need another loop for small entities because small entities don't spawn further
    }
}

void Game::sEnemySpawner() {
    const auto elapsedTime = m_currentFrame - m_lastEnemySpawnTime;
    if (elapsedTime > m_enemyConfig.SI)
    {
        if (elapsedTime % 7 == 0)
            spawnEnemy("spazbit");
        else
            spawnEnemy("enemy");
    }
}

void Game::sGUI() {
    ImGui::Begin("Geometry Wars");

    guiOptions();
    guiLogging();
    guiSpawner();
    guiEntityTable();

    ImGui::End();
}

void Game::sRender() {
    m_window.clear();

    // Get all entities
    auto entities = m_entities.getEntities();
    // Iterate through all entities
    for (const auto& entity : entities) {
        // Only draw entities that have both transform and shape components
        if (entity->has<CTransform>() && entity->has<CShape>()) {
            auto& transform = entity->get<CTransform>();
            auto& shape = entity->get<CShape>();

            // update rotation
            transform.angle += 1;
            transform.angle = std::fmod(transform.angle, 360.0f);

            // Update position and rotation
            shape.circle.setPosition(transform.pos);
            shape.circle.setRotation(sf::degrees(transform.angle));

            // Draw the entity
            m_window.draw(shape.circle);
        }
    }

    // draw the ui last
    std::stringstream ss;
    ss << "Score: ";
    ss << m_score;
    ss << "\n";
    m_text.setString(ss.str());
    m_window.draw(m_text);

    ImGui::SFML::Render(m_window);


    m_window.display();
}

void Game::sUserInput() {
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
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().up = true;
                    }
                    break;
                case sf::Keyboard::Scancode::P:
                    m_paused = !m_paused;
                    break;
                case sf::Keyboard::Scancode::S:
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().down = true;
                    }
                    break;
                case sf::Keyboard::Scancode::A:
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().left = true;
                    }
                    break;
                case sf::Keyboard::Scancode::D:
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().right = true;
                    }
                    break;
                case sf::Keyboard::Scancode::Escape:
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
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().up = false;
                    }
                    break;
                case sf::Keyboard::Scancode::S:
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().down = false;
                    }
                    break;
                case sf::Keyboard::Scancode::A:
                    if (player() && player()->has<CInput>()) {
                        player()->get<CInput>().left = false;
                    }
                    break;
                case sf::Keyboard::Scancode::D:
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
                // TODO: call spawnBullet here
                spawnBullet(Vec2f(static_cast<float>(mouseEvent->position.x),
                            static_cast<float>(mouseEvent->position.y)));
            }
            else if (mouseEvent->button == sf::Mouse::Button::Right) {
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

void Game::guiSpawner()
{
    if (ImGui::CollapsingHeader("Entity Spawner"))
    {
        if (ImGui::Button("Spawn Enemy")) spawnEnemy("enemy");
        if (ImGui::Button("Spawn spazbit")) spawnEnemy("spazbit");

    }
}


void Game::guiLogging() const
{
    if (ImGui::CollapsingHeader("Score"))
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), std::to_string(m_score).c_str());
    if (ImGui::CollapsingHeader("Logging"))
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), ssDebug.str().c_str());
}

void Game::guiOptions()
{
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

        ImGui::BeginGroup();
        ImGui::Checkbox("Enemy Spawning", &m_isEnemeySpawnDisabled);
        ImGui::Checkbox("Movement", &m_isMovementDisabled);
        ImGui::Checkbox("Collisions", &m_isCollisionDisabled);
        ImGui::Checkbox("Lifespan", &m_isLifespanDisabled);

        // Array of string labels matching your enum values in the same order
        const char* interpolationTypeLabels[] = {
            "Ease Out Elastic",
            "Ease Out Sine",
            "Ease In Elastic",
            "Ease In Sine",
            "Ease In Out Sine",
            "Ease In Out Elastic",
            "Ease In Out Expo",
            "Ease In Expo",
        };

        // Create the combo box with your enum
        int currentItem = static_cast<int>(m_debugEasing); // Use your enum variable
        if (ImGui::Combo("Interpolation Type", &currentItem, interpolationTypeLabels, IM_ARRAYSIZE(interpolationTypeLabels)))
        {
            // Update your enum variable when selection changes
            m_debugEasing = static_cast<InterpolationType>(currentItem);
        }

        ImGui::EndGroup();
    }
  }

void Game::guiEntityTable()
{
    if (ImGui::CollapsingHeader("Entity List"))
    {
        // Display the entities in a table
        ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | 
                                    ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH;
        
        if (ImGui::BeginTable("Entities", 8, tableFlags)) // Changed from 6 to 7 columns to include Lifespan
        {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("Lifespan", ImGuiTableColumnFlags_WidthFixed, 70.0f); // New column for lifespan
            ImGui::TableSetupColumn("Number of Point", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("Remove", ImGuiTableColumnFlags_WidthFixed, 70.0f);
            ImGui::TableSetupColumn("Children", ImGuiTableColumnFlags_WidthFixed, 70.0f);
            ImGui::TableHeadersRow();

            for (const auto& entity : m_entities.getEntities())
            {
                if (!entity->isActive()) continue;
                
                ImGui::TableNextRow();
                
                // Column 0: Entity ID
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%zu", entity->id());
                
                // Column 1: Entity Type (tag)
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", entity->tag().c_str());
                
                // Only display position if entity has a transform component
                if (entity->has<CTransform>())
                {
                    const auto& transform = entity->get<CTransform>();
                    
                    // Column 2: X Position
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%.1f", transform.pos.x);
                    
                    // Column 3: Y Position
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%.1f", transform.pos.y);
                }
                else
                {
                    // If no transform, display N/A
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("N/A");
                    
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("N/A");
                }
                
                // Column 4: Lifespan remaining
                ImGui::TableSetColumnIndex(4);
                if (entity->has<CLifespan>())
                {
                    const auto& lifespan = entity->get<CLifespan>();
                    ImGui::Text("%d/%d", lifespan.remaining, lifespan.lifespan);
                }
                else
                {
                    ImGui::Text("N/A");
                }

                // Column 5: Number of points
                ImGui::TableSetColumnIndex(5);
                if (entity->has<CShape>())
                {
                    const auto& shape = entity->get<CShape>();
                    ImGui::Text("%d", shape.getPointCount());
                } else
                {
                    ImGui::Text("N/A");
                }
                
                // Column 6: Remove button
                ImGui::TableSetColumnIndex(6);
                
                // Create a unique label for the button using entity ID
                std::string buttonLabel = "Remove##" + std::to_string(entity->id());
                
                // Add the Remove button
                if (ImGui::Button(buttonLabel.c_str()))
                {
                    // Mark the entity for deletion when button is clicked
                    entity->destroy();
                }

                // Column 6: Remove button
                ImGui::TableSetColumnIndex(7);

                // Create a unique label for the button using entity ID
                std::string btnChildren = "children##" + std::to_string(entity->id());

                // Add the Remove button
                if (ImGui::Button(btnChildren.c_str()))
                {
                    // Mark the entity for deletion when button is clicked
                    spawnSmallEnemies(entity);
                }
            }
            
            ImGui::EndTable();
        }
    }
}

// Add to Game.cpp
std::shared_ptr<Entity> Game::createEntity(const std::string& tag,
                                         const Vec2f& position,
                                         int shapeRadius,
                                         size_t vertexCount,
                                         const sf::Color& fillColor,
                                         const sf::Color& outlineColor,
                                         float outlineThickness,
                                         const Vec2f& velocity,
                                         int lifespan,
                                         int collisionRadius,
                                         int score,
                                         InterpolationType const easing)
{
    auto entity = m_entities.addEntity(tag);

    // Add transform component
    entity->add<CTransform>(position, velocity, 0.0f);

    // Add shape component
    entity->add<CShape>(shapeRadius, vertexCount, fillColor, outlineColor, outlineThickness);

    // Add optional components based on provided parameters
    if (lifespan > 0) {
        entity->add<CLifespan>(lifespan);
        entity->get<CLifespan>().setEasingType(easing);
    }

    if (collisionRadius > 0) {
        entity->add<CCollision>(collisionRadius);
    }

    if (score > 0) {
        entity->add<CScore>(score);
    }

    return entity;
}