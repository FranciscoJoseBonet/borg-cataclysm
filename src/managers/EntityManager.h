#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "../entities/Entity.h"

// Esta es una clase encargada de tener todas las entidades del juego en un solo lugar
// Se encarga de actualizarlas, dibujarlas y borrarlas cuando mueren
// La proxima feature del juego será la implementacion de la separacion de estas clases para no tenerlas todas en el mismo vector
class EntityManager
{
private:
    // Lista principal de entidades activas de todo el game
    std::vector<std::unique_ptr<Entity>> entities;

    // Aqui guardamos las entidades nuevas creadas DURANTE el update
    // Es para no meter entidades durante la lectura del vector
    std::vector<std::unique_ptr<Entity>> pendingEntities;

public:
    EntityManager() = default;

    // Borramos constructor de copia y asignacion para evitar duplicar punteros unicos
    EntityManager(const EntityManager &) = delete;
    EntityManager &operator=(const EntityManager &) = delete;

    void update(float dt)
    {
        // 1. Integrar las entidades pendientes de la frame anterior
        for (auto &e : pendingEntities)
        {
            entities.push_back(std::move(e));
        }
        pendingEntities.clear();

        // 2. Actualizar todas las entidades activas
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

    // Elimina de la memoria las entidades marcadas como muertas
    // Se suele llamar al final del frame
    void refresh()
    {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                           [](const std::unique_ptr<Entity> &e)
                           {
                               return !e->isAlive();
                           }),
            entities.end());
    }

    // Template para crear entidades facilmente
    // Ejemplo de uso: manager.add<Enemy>(posicion, textura);
    template <typename T, typename... Args>
    T &add(Args &&...args)
    {
        auto uPtr = std::make_unique<T>(std::forward<Args>(args)...);
        T *ptr = uPtr.get();

        // Guardamos en la lista de espera, no en la principal
        pendingEntities.push_back(std::move(uPtr));

        return *ptr;
    }

    void clear()
    {
        pendingEntities.clear();
        entities.clear();
    }

    std::vector<std::unique_ptr<Entity>> &getEntities()
    {
        return entities;
    }
};