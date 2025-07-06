#include <SFML/Graphics.hpp>
#include "Game.h"
#include "shapes/Shape.h"
#include "vec2/Vec2.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <functional>
#include <vector>

int main()
{
    const std::string configPath = "assets/bin/config.txt";
    Game game(configPath);
    game.run();
    return 0;
}
