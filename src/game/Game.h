// Game class header - Main game engine implementation
#pragma once

#include <SFML/Graphics.hpp>
#include "../entitymanager/EntityManager.h"
#include "../systems/Systems.h"
#include "Vec2.h"
#include <sstream>


struct PlayerConfig{int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig {int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX;};
struct BulletConfig{int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };
// W = width, H = height, FL = frame limit, FS = full screen ( 1 = true, 0 = false)
struct WindowConfig{int W, H, FL, FS;};
struct FontConfig{std::string fontFile; int fontSize; int R, G, B;};


class Game
{
    sf::RenderWindow    m_window; // the window we are rendering to
    EntityManager       m_entities; // the entity manager
    sf::Font            m_font;   // the font
    sf::Text            m_text;   // the text to display the score

    PlayerConfig        m_playerConfig;
    EnemyConfig         m_enemyConfig;
    BulletConfig        m_bulletConfig;
    WindowConfig        m_windowConfig;
    FontConfig          m_fontConfig;

    Interpolate      m_interpolations;
    sf::Clock        m_deltaClock;
    int              m_score                 = 0;
    int              m_currentFrame          = 0;
    int              m_lastEnemySpawnTime    = 0;
    bool             m_paused                = false;
    bool             m_running               = true;
    bool             m_isEnemeySpawnDisabled = false;
    bool             m_isMovementDisabled    = false;
    bool             m_isCollisionDisabled   = false;
    bool             m_isLifespanDisabled    = false;
    std::shared_ptr<Entity> m_player;
    std::stringstream ssDebug;

    InterpolationType m_debugEasing = EASEIN_SINE;
    void init(const std::string & config); // Initialize the game with a config file
    void setPaused(bool paused);

    // System functions
    void sMovement();
    void sUserInput();
    void sLifespan();
    void sRender();
    void sGUI();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy (const std::string& type);
    void spawnSmallEnemies (std::shared_ptr<Entity> const& e) ;
    void spawnBullet (const Vec2f & target);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
    void spazbitMovement(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> player();

    // Helper functions
    void parseConfig(const std::string & type, const std::string & values);
    void guiOptions();
    void guiLogging() const;
    void guiSpawner();
    void guiEntityTable();

    // Add to Game.h in the private section
    std::shared_ptr<Entity> createEntity(const std::string& tag,
                                        const Vec2f& position,
                                        int shapeRadius,
                                        size_t vertexCount,
                                        const sf::Color& fillColor,
                                        const sf::Color& outlineColor,
                                        float outlineThickness,
                                        const Vec2f& velocity = Vec2f(0.0f, 0.0f),
                                        int lifespan = 0,
                                        int collisionRadius = 0,
                                        int score = 0,
                                        InterpolationType easing = EASEIN_SINE);


public:
    explicit Game(const std::string & config);
    void run();
};