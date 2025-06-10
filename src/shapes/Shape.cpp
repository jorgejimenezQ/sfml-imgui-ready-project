//
// Created by Jorge Jimenez on 6/3/25.
//

#include "Shape.h"

namespace cmakelearn {

Shape::Shape() : mShape(100.f) {
    mShape.setFillColor(sf::Color::Green);
    mShape.setOutlineColor(sf::Color::White);
    mShape.setOutlineThickness(2.f);
    mShape.setPosition(sf::Vector2f(0.f, 0.f));
}

Shape::~Shape() = default;

void Shape::setPosition(float x, float y) {
    mShape.setPosition(sf::Vector2f(x, y));
}

void Shape::setFillColor(const sf::Color& color) {
    mShape.setFillColor(color);
}

void Shape::setOutlineColor(const sf::Color& color) {
    mShape.setOutlineColor(color);
}

void Shape::setOutlineThickness(float thickness) {
    mShape.setOutlineThickness(thickness);
}

void Shape::draw(sf::RenderTarget& target, sf::RenderStates states) {
    target.draw(mShape, states);
}

} // namespace cmakelearn