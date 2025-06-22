//
// Created by Jorge Jimenez on 6/11/25.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

template<typename T>
class Vec2 {
public:
    T x = 0;
    T y = 0;

    /// Default constructor, sets x and y to 0
    Vec2() = default;
    /// Constructor with x and y values
    /// \param xin The x value to set
    /// \param yin The y value to set
    Vec2(T xin, T yin)
        : x(xin), y(yin) {}
    // Constructor to convert sf::Vector2f to Vec2
    Vec2(const sf::Vector2f vec)
        : x(vec.x), y(vec.y) {}

    // allow automatic conversion to sf::Vector2
    // this lets us pass Vec2 in sfml fucntions
    operator sf::Vector2<T>()
    {
        return sf::Vector2<T>(x, y);
    }

    Vec2 operator + (const Vec2& rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2 operator - (const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    Vec2 operator / (const T val) const
    {
        return Vec2(x / val, y / val);
    }

    Vec2 operator * (const T val) const
    {
        return Vec2(x * val, y * val);
    }

    bool operator == (const Vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator != (const Vec2& rhs) const
    {
        return !(x == rhs.x && y == rhs.y);
    }

    void operator += (const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    void operator -= (const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    void operator *= (const T val)
    {
        x *= val;
        y *= val;
    }

    void operator /= (const T val)
    {
        x /= val;
        y /= val;
    }

    float dist(const Vec2& rhs) const
    {
        return sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
    }
};

using Vec2f = Vec2<float>;
