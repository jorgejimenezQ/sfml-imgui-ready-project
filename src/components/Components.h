//
// Created by Jorge Jimenez on 6/11/25.
//

#pragma once

#include "../vec2/Vec2.h"
#include <SFML/Graphics.hpp>


class Component
{
public:
    bool exists = false;
};

class CTransform : public Component
{
public:
    Vec2f pos       = { 0.0, 0.0};
    Vec2f velocity  = { 0.0, 0.0};
    float angle     = 0;

    CTransform() = default;
    CTransform(const Vec2f & p, const Vec2f & v, const float a)
            : pos(p), velocity(v), angle(a) {}
};

class CShape : public Component
{
public:
    sf::CircleShape circle;

    CShape() = default;
    CShape(const float radius, size_t points, const sf::Color & fill, const sf::Color & outline, const float thickness)
    {
        circle.setRadius(radius);
        circle.setPointCount(points);
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);

        // Set the origin to the center to radius, radius
        const sf::Vector2f center = { radius, radius };
        circle.setOrigin(center);
    }
};

class CCollision : public Component
{
public:
    float radius = 0;

    CCollision() = default;
    explicit CCollision(const float r)
        : radius(r) {}

};

class CScore : public Component
{
public:
    int score = 0;

    CScore() = default;
    explicit CScore(const int s)
        : score(s){}
};

class CLifespan : public Component
{
public:
    int lifespan = 0;
    int remaining = 0;

    CLifespan() = default;
    explicit CLifespan(const int totalLifespan)
        : lifespan(totalLifespan), remaining(totalLifespan){}
};

class CInput : public Component
{
public:
    bool up = false;
    bool left = false;
    bool right = false;
    bool down = false;
    bool shoot = false;

    CInput() = default;
};
