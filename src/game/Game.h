#pragma once

#include <SFML/Graphics.hpp>
#include "../entitymanager/EntityManager.h"
#include "Vec2.h"

struct PlayerConfig{int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig {int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX;};
struct BulletConfig{int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
    sf::RenderWindow    m_window; // the window we are rendering to
    EntityManager       m_entities; // the entity manager
    sf::Font            m_font;   // the font
    sf::Text            m_text;   // the text to display the score
    PlayerConfig        m_playerConfig;
    EnemyConfig         m_enemyConfig;
    BulletConfig        m_bulletConfig;
    sf::Clock           m_deltaClock;
    int                 m_score = 0;
    int                 m_currentFrame = 0;
    int                 m_lastEnemySpawnTime = 0;
    bool                m_paused = false;
    bool                m_running = true;
    std::shared_ptr<Entity> m_player;

    void init(const std::string & config); // Initialize the game with a config file
    void setPaused(bool paused);

    void sMovement();
    void sUserInput();
    void sLifespan();
    void sRender();
    void sGUI();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy ();
    void spawnSmallEnemies (std::shared_ptr<Entity> entity) ;
    void spawnBullet (std::shared_ptr<Entity> entity, const Vec2f & target);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
    Game(const std::string & config);
    void run();
};