//
// Created by Jorge Jimenez on 6/11/25.
//

#pragma once

#include "../vec2/Vec2.h"
#include <SFML/Graphics.hpp>

enum InterpolationType
{
    EASEOUT_ELASTIC,
    EASEOUT_SINE,
    EASEIN_ELASTIC,
    EASEIN_SINE,
    EASEINOUT_SINE,
    EASEINOUT_ELASTIC,
    EASEINOUT_EXPO,
    EASEIN_EXPO,
};

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

    float getRadius() const
    {
        return circle.getRadius();
    }

    size_t getPointCount() const
    {
        return circle.getPointCount();
    }

    void setFillColor(const sf::Color color)
    {
        circle.setFillColor(color);
    }

    void setOutlineColor(const sf::Color color)
    {
        circle.setOutlineColor(color);
    }

    sf::Color getOutlineColor() const
    {
        return circle.getOutlineColor();
    }
    sf::Color getFillColor() const
    {
        return circle.getFillColor();
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

// class CShooting : public Component
// {
// public:
//     bool isShooting = false;
//     BulletType bulletType = USER;
//     CShooting() = default;
// };

class CScore : public Component
{
public:
    int score = 0;

    CScore() = default;
    explicit CScore(const int s)
        : score(s){}
};

class CInterpolation : public Component
{
public:
    InterpolationType type = EASEIN_SINE;
    explicit
    CInterpolation(InterpolationType interpolationType):
        type(interpolationType){}
};

class CLifespan : public Component
{
    InterpolationType easeing = EASEIN_SINE;

public:
    int lifespan = 0;
    int remaining = 0;

    CLifespan() = default;
    explicit CLifespan(const int totalLifespan)
        : lifespan(totalLifespan), remaining(totalLifespan){}

    void setEasingType(InterpolationType const type)
    {
        easeing = type;
    }

    [[nodiscard]]
    InterpolationType getEasing() const
    {
        return easeing;
    }
};

class CSpazJump : public Component
{
public:
    float distanceTraveled = 0.0f;   // what you were calling “progress”
    float distanceToTravel  = 0.0f;   // what you were calling “dist”

    CSpazJump() = default;
    explicit
    CSpazJump(const float distToJump)
        : distanceToTravel(distToJump)
    {
    }
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
