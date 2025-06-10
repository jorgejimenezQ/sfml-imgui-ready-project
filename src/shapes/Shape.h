//
// Created by Jorge Jimenez on 6/3/25.
//

#ifndef CMAKELEARN_SHAPE_H
#define CMAKELEARN_SHAPE_H

#include <SFML/Graphics.hpp>

namespace cmakelearn {

class Shape {
public:
    Shape();
    ~Shape();
    
    // Set the position of the shape
    void setPosition(float x, float y);
    
    // Set the fill color of the shape
    void setFillColor(const sf::Color& color);
    
    // Set the outline color of the shape
    void setOutlineColor(const sf::Color& color);
    
    // Set the outline thickness
    void setOutlineThickness(float thickness);
    
    // Draw the shape to a render target
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
    
private:
    sf::CircleShape mShape;
};

} // namespace cmakelearn

#endif // CMAKELEARN_SHAPE_H