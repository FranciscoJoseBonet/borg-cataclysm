#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "../entities/Entity.h"

using namespace std;

class EntityManager
{
private:
    vector<unique_ptr<Entity>> entities;
    vector<unique_ptr<Entity>> pendingEntities;

public:
    EntityManager() = default;

    EntityManager(const EntityManager &) = delete;
    EntityManager &operator=(const EntityManager &) = delete;

    void update(float dt)
    {
        for (auto &e : pendingEntities)
        {
            entities.push_back(move(e));
        }
        pendingEntities.clear();

        for (auto &e : entities)
        {
            if (e->isAlive())
            {
                e->update(dt);
            }
        }
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default)
    {
        for (auto &e : entities)
        {
            if (e->isAlive())
            {
                target.draw(*e, states);
            }
        }
    }

    void refresh()
    {
        entities.erase(
            remove_if(entities.begin(), entities.end(),
                      [](const unique_ptr<Entity> &e)
                      {
                          return !e->isAlive();
                      }),
            entities.end());
    }

    template <typename T, typename... Args>
    T &add(Args &&...args)
    {
        auto uPtr = make_unique<T>(forward<Args>(args)...);
        T *ptr = uPtr.get();
        pendingEntities.push_back(move(uPtr));
        return *ptr;
    }

    void clear()
    {
        pendingEntities.clear();
        entities.clear();
    }
};