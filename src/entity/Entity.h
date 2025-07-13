#pragma once

#include "../components/Components.h"
#include <string>
#include <tuple>

class EntityManager;

/**
 * @brief Tuple containing all possible component types for an entity
 * 
 * This defines all component types that can be attached to any entity in the system.
 * Components are stored as a tuple to allow for efficient type-based access.
 */
using ComponentTuple = std::tuple<
    CTransform,
    CShape,
    CCollision,
    CInput,
    CScore,
    CLifespan,
    CSpazJump
>;

/**
 * @brief Entity class represents a game object with component-based architecture
 * 
 * Entities are containers for components. Each entity has a unique ID, a tag for
 * identification, and can have different components attached to it that define
 * its behavior and properties. The entity itself has minimal behavior beyond 
 * managing its components and lifecycle.
 * 
 * @example
 * // Entities should be created through EntityManager
 * auto entity = entityManager.addEntity("player");
 * 
 * // Add components to the entity
 * entity->add<CTransform>(Vec2f(100, 100));
 * entity->add<CShape>(32, 8, sf::Color::Blue, sf::Color::Red, 2);
 * 
 * // Check for components and use them
 * if (entity->has<CTransform>() && entity->has<CShape>()) {
 *     auto& transform = entity->get<CTransform>();
 *     transform.pos.x += 10;
 * }
 */
class Entity
{
    friend class EntityManager;

    ComponentTuple  components;     ///< Stores all components for this entity
    bool            active = true;  ///< Whether this entity is active in the game world
    std::string     entityTag    = "dfault"; ///< Identifying tag for this entity type
    size_t          entityId     = 0;        ///< Unique identifier for this entity

    /**
     * @brief Constructor is private - entities should only be created via EntityManager
     * 
     * @param id Unique identifier assigned by EntityManager
     * @param entityTag String tag for identifying entity type
     * 
     * @example
     * // Entities should not be constructed directly
     * auto entity = entityManager.addEntity("player"); // Correct way to create
     */
    Entity(const size_t id, std::string entityTag)
        : entityTag(std::move(entityTag)), entityId(id)
    {}

public:
    /**
     * @brief Checks if this entity is active in the game world
     * 
     * Inactive entities will be removed by the EntityManager during the next update.
     * 
     * @return true if the entity is active, false otherwise
     * 
     * @example
     * // Check if entity is still active before using it
     * if (entity->isActive()) {
     *     // Safe to use the entity
     *     entity->get<CTransform>().pos.x += 5;
     * }
     */
    [[nodiscard]] bool isActive () const
    {
        return active;
    }

    /**
     * @brief Marks this entity for deletion
     * 
     * Sets active to false, which signals the EntityManager that this entity
     * should be removed during the next update cycle.
     * 
     * @example
     * // Mark an entity for deletion when it's no longer needed
     * if (bullet->get<CLifespan>().remaining <= 0) {
     *     bullet->destroy();  // Will be cleaned up by EntityManager
     * }
     */
    void destroy()
    {
        active = false;
    }

    /**
     * @brief Gets the unique identifier for this entity
     * 
     * @return size_t The entity's unique ID
     * 
     * @example
     * // Get the entity's ID for debugging or comparison
     * size_t id = entity->id();
     * std::cout << "Entity ID: " << id << std::endl;
     * 
     * // Check if two entity references point to the same entity
     * if (entity1->id() == entity2->id()) {
     *     std::cout << "Same entity!" << std::endl;
     * }
     */
    [[nodiscard]] size_t id() const
    {
        return entityId;
    }

    /**
     * @brief Gets the tag string for this entity
     * 
     * The tag is useful for identifying the entity type (e.g., "player", "enemy")
     * 
     * @return const std::string& Reference to the entity's tag
     * 
     * @example
     * // Use tag to identify entity type for game logic
     * if (entity->tag() == "player") {
     *     // Apply player-specific logic
     * } else if (entity->tag() == "enemy") {
     *     // Apply enemy-specific logic
     * }
     */
    [[nodiscard]] const std::string& tag() const
    {
        return entityTag;
    }

    /**
     * @brief Checks if the entity has a specific component
     * 
     * @tparam T The component type to check for
     * @return true if the component exists and is active
     * 
     * @example
     * // Check if entity has necessary components before using them
     * if (entity->has<CTransform>() && entity->has<CInput>()) {
     *     // Safe to access these components
     *     auto& input = entity->get<CInput>();
     *     auto& transform = entity->get<CTransform>();
     *     
     *     if (input.up) {
     *         transform.velocity.y = -5.0f;
     *     }
     * }
     */
    template <typename T>
    [[nodiscard]] bool has() const
    {
        return get<T>().exists;
    }

    /**
     * @brief Adds or replaces a component on this entity
     * 
     * @tparam T The component type to add
     * @tparam TArgs Types of arguments to forward to the component constructor
     * @param mArgs Arguments to forward to the component constructor
     * @return T& Reference to the newly created component
     * 
     * @example
     * // Add a transform component to position the entity
     * entity->add<CTransform>(Vec2f(100, 200), Vec2f(0, 0), 0.0f);
     * 
     * // Add a shape component with radius 32, 8 vertices, colors, and outline thickness
     * entity->add<CShape>(32.0f, 8, sf::Color::Blue, sf::Color::Red, 2.0f);
     * 
     * // Add and configure component in one step
     * auto& collision = entity->add<CCollision>(32.0f);
     * collision.radius = 30.0f;  // Adjust after creation
     */
    template <typename T, typename... TArgs>
    T& add(TArgs&&... mArgs)
    {
        auto& component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.exists = true;
        return component;
    }

    /**
     * @brief Gets a reference to a component of the specified type
     * 
     * @tparam T The component type to retrieve
     * @return T& Reference to the component
     * 
     * @example
     * // Get and modify a component
     * auto& transform = entity->get<CTransform>();
     * transform.pos.x += 10;
     * transform.pos.y -= 5;
     * transform.angle += 0.1f;
     * 
     * // Access component properties
     * auto& shape = entity->get<CShape>();
     * shape.circle.setFillColor(sf::Color::Green);
     */
    template<typename T>
    [[nodiscard]]
    T& get()
    {
        return std::get<T>(components);
    }

    /**
     * @brief Gets a const reference to a component of the specified type
     * 
     * @tparam T The component type to retrieve
     * @return const T& Const reference to the component
     * 
     * @example
     * // For read-only access to components (in const methods)
     * void printPosition(const Entity& entity) {
     *     if (entity.has<CTransform>()) {
     *         const auto& transform = entity.get<CTransform>();
     *         std::cout << "Position: (" << transform.pos.x << ", " 
     *                   << transform.pos.y << ")" << std::endl;
     *     }
     * }
     */
    template<typename T>
    [[nodiscard]]
    const T& get() const
    {
        return std::get<T>(components);
    }

    /**
     * @brief Removes a component from this entity
     * 
     * Sets the component to its default state and marks it as non-existent.
     * 
     * @tparam T The component type to remove
     * 
     * @example
     * // Remove a component when it's no longer needed
     * if (entity->has<CLifespan>()) {
     *     entity->remove<CLifespan>();
     * }
     * 
     * // Reset a component to default state
     * entity->remove<CInput>();  // Clear all input flags
     * entity->add<CInput>();     // Re-add with defaults
     */
    template<typename T>
    void remove()
    {
        get<T>() = T();
    }
};