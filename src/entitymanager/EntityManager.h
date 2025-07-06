//
// Created by Jorge Jimenez on 6/15/25.
//

#pragma once

#include "../entity/Entity.h"
#include <vector>
#include <map>
#include <memory>

using EntityVec = std::vector<std::shared_ptr<Entity>>;


class EntityManager
{
    EntityVec                           entitiesList;
    EntityVec                           entitiesToAdd;
    std::map<std::string, EntityVec>    entityMap;
    size_t                              totalEntities = 0;

    void removeDeadEntities(EntityVec& vec)
    {
        //TODO: remove dead entities
    }

public:
     EntityManager() = default;
     void update()
     {
         // TODO: add entities from entitiesToAdd the proper location(s)
         //        - add them to the vector of all entities
         //        - add them to the vector inside the map, with the tag as a key
        for (const auto& e : entitiesToAdd)
        {
            entitiesList.push_back(e);
            // entityMap[e->tag()].push_back(e);
        }

         entitiesToAdd.clear();

         // remove dead entities from hte vector of all entities
         removeDeadEntities(entitiesList);

         // remove dead entities from each vector in the entity map
         // c++20 way of iterating through [key, value] pairs in a map
         for (auto& [tag, entityVec] : entityMap)
         {
             removeDeadEntities(entityVec);
         }
     }

    std::shared_ptr<Entity> addEntity(const std::string& tag)
    {
        //create the entity shared pointer
        auto entity = std::shared_ptr<Entity>(new Entity(totalEntities++, tag));

        // add it to the vec of all entities
        entitiesToAdd.push_back(entity);

        // add it to the entity map
        if (entityMap.find(tag) == entityMap.end())
        {
            entityMap[tag] = EntityVec();
        }

        entityMap[tag].push_back(entity);
        return entity;
    }

    const EntityVec& getEntities()
    {
        return entitiesList;
    }

    const EntityVec& getEntities(const std::string& tag)
    {
        if (entityMap.find(tag) == entityMap.end())
        {
            entityMap[tag] = EntityVec();
        }

        return entityMap[tag];
    }

    const std::map<std::string, EntityVec>& getEntityMap()
    {
        return entityMap;
    }
};
