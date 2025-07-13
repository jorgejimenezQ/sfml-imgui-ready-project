#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

/**
 * @brief A 2D vector template class compatible with SFML
 * 
 * This class provides a template-based 2D vector implementation that works seamlessly with SFML.
 * It supports common vector operations like addition, subtraction, multiplication, and division,
 * as well as utility functions for distance calculation and normalization.
 * 
 * @tparam T The numeric type for vector components (float, int, double, etc.)
 * 
 * Example usage:
 * @code
 * // Create vectors
 * Vec2f position(100.0f, 200.0f);
 * Vec2f velocity(5.0f, -3.0f);
 * 
 * // Vector operations
 * Vec2f newPosition = position + velocity;       // Addition
 * Vec2f direction = position - Vec2f(0.0f, 0.0f); // Subtraction
 * Vec2f scaledVelocity = velocity * 2.0f;       // Scaling
 * 
 * // Automatic conversion to SFML vector
 * sf::CircleShape circle;
 * circle.setPosition(position); // Vec2f converts to sf::Vector2f
 * 
 * // Distance calculation
 * float distanceToOrigin = position.dist(Vec2f(0.0f, 0.0f));
 * 
 * // Normalization
 * Vec2f normalizedDirection = Vec2f::normalize(direction);
 * @endcode
 */
template<typename T>
class Vec2 {
public:
    T x = 0;  ///< X component of the vector
    T y = 0;  ///< Y component of the vector

    /// Default constructor, sets x and y to 0
    Vec2() = default;
    
    /**
     * @brief Constructor with x and y values
     * @param xin The x value to set
     * @param yin The y value to set
     */
    Vec2(T xin, T yin)
        : x(xin), y(yin) {}
    
    /**
     * @brief Constructor to convert sf::Vector2f to Vec2
     * @param vec The SFML vector to convert
     */
    Vec2(const sf::Vector2f vec)
        : x(vec.x), y(vec.y) {}

    /**
     * @brief Conversion operator to sf::Vector2<T>
     * 
     * This allows automatic conversion to SFML vectors when passing
     * Vec2 instances to SFML functions.
     * 
     * @return Equivalent sf::Vector2<T>
     */
    operator sf::Vector2<T>()
    {
        return sf::Vector2<T>(x, y);
    }

    /**
     * @brief Vector addition
     * @param rhs Right-hand side vector
     * @return Sum of the two vectors
     */
    Vec2 operator + (const Vec2& rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    /**
     * @brief Vector subtraction
     * @param rhs Right-hand side vector
     * @return Difference of the two vectors
     */
    Vec2 operator - (const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    /**
     * @brief Vector scalar division
     * @param val Scalar value to divide by
     * @return Vector divided by scalar
     */
    Vec2 operator / (const T val) const
    {
        return Vec2(x / val, y / val);
    }

    /**
     * @brief Vector scalar multiplication
     * @param val Scalar value to multiply by
     * @return Vector multiplied by scalar
     */
    Vec2 operator * (const T val) const
    {
        return Vec2(x * val, y * val);
    }

    /**
     * @brief Vector equality comparison
     * @param rhs Right-hand side vector
     * @return true if vectors are equal, false otherwise
     */
    bool operator == (const Vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    /**
     * @brief Vector inequality comparison
     * @param rhs Right-hand side vector
     * @return true if vectors are not equal, false otherwise
     */
    bool operator != (const Vec2& rhs) const
    {
        return !(x == rhs.x && y == rhs.y);
    }

    /**
     * @brief Vector addition assignment
     * @param rhs Right-hand side vector
     */
    void operator += (const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    /**
     * @brief Vector subtraction assignment
     * @param rhs Right-hand side vector
     */
    void operator -= (const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    /**
     * @brief Vector scalar multiplication assignment
     * @param val Scalar value to multiply by
     */
    void operator *= (const T val)
    {
        x *= val;
        y *= val;
    }

    /**
     * @brief Vector scalar division assignment
     * @param val Scalar value to divide by
     */
    void operator /= (const T val)
    {
        x /= val;
        y /= val;
    }

    /**
     * @brief Calculate Euclidean distance between two vectors
     * @param rhs Right-hand side vector
     * @return Distance between this vector and rhs
     */
    float dist(const Vec2& rhs) const
    {
        return sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
    }

    /**
     * @brief Returns a normalized version of the given vector (unit vector with same direction)
     * 
     * A normalized vector has the same direction but a length of 1.
     * If the input vector has zero length, a zero vector is returned.
     * 
     * @param vec The vector to normalize
     * @return A unit vector in the same direction as the input
     */
    [[nodiscard]] static Vec2 normalize(const Vec2& vec)
    {
        float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        if (length > 0) {
            return Vec2(vec.x / length, vec.y / length);
        }
        return Vec2(0, 0);
    }
};

/// Type alias for Vec2<float>, the most commonly used type
using Vec2f = Vec2<float>;

/// Type alias for Vec2<int>, useful for grid positions or pixel coordinates
using Vec2i = Vec2<int>;
