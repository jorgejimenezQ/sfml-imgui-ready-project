#pragma once

#include "../components/Components.h"
#include <string>
#include <tuple>

class EntityManager;

using ComponentTuple = std::tuple<
    CTransform,
    CShape,
    CCollision,
    CInput,
    CScore,
    CLifespan
>;

class Entity
{
    friend class EntityManager;

    ComponentTuple  components;
    bool            active = true;
    std::string     entityTag    = "dfault";
    size_t          entityId     = 0;

    Entity(const size_t id, std::string entityTag)
        : entityTag(std::move(entityTag)), entityId(id)
    {}

public:
    [[nodiscard]] bool isActive () const
    {
        return active;
    }

    void destroy()
    {
        active = false;
    }

    [[nodiscard]] size_t id() const
    {
        return entityId;
    }

    [[nodiscard]] const std::string& tag() const
    {
        return entityTag;
    }

    template <typename T>
    [[nodiscard]] bool has() const
    {
        return get<T>().exists;
    }

    template <typename T, typename... TArgs>
    T& add(TArgs&&... mArgs)
    {
        auto& component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.exists = true;
        return component;
    }

    template<typename T>
    [[nodiscard]] T& get()
    {
        return std::get<T>(components);
    }

    template<typename T>
    [[nodiscard]] const T& get() const
    {
        return std::get<T>(components);
    }

    template<typename T>
    void remove()
    {
        get<T>() = T();
    }
};