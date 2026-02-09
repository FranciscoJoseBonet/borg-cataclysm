#include "CollisionManager.h"
#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Enemy.h"
#include "../entities/projectiles/Projectile.h"
#include "../entities/items/PowerUp.h"
#include <iostream>

void CollisionManager::checkCollisions(EntityManager &entityManager)
{
    auto &entities = entityManager.getEntities();

    // Doble bucle optimizado
    // El bucle interno empieza en 'itA + 1' (std::next) para evitar:
    // 1. Comparar una entidad consigo misma
    // 2. Comparar A con B y luego B con A
    for (auto itA = entities.begin(); itA != entities.end(); ++itA)
    {
        for (auto itB = std::next(itA); itB != entities.end(); ++itB)
        {
            Entity *a = itA->get();
            Entity *b = itB->get();

            // Si alguno esta muerto, no procesamos colision
            if (!a->isAlive() || !b->isAlive())
                continue;

            // Filtros especiales del Jugador Para cuando es invulnerable
            SpaceShip *player = nullptr;
            Entity *other = nullptr;

            if (auto *s = dynamic_cast<SpaceShip *>(a))
            {
                player = s;
                other = b;
            }
            else if (auto *s = dynamic_cast<SpaceShip *>(b))
            {
                player = s;
                other = a;
            }

            if (player && player->isInvulnerableState())
            {
                // Si es invulnerable, ignora colisiones con Aliens o sus balas
                if (other->getFaction() == Faction::Alien)
                    continue;
            }

            // Proyectil contra Proyectil no hacen nada (Si quisiera destruir un proyectil disparandole se modificaria aca)
            if (dynamic_cast<Projectile *>(a) && dynamic_cast<Projectile *>(b))
            {
                continue;
            }

            // Lógica de detección
            // Caso 1: Facciones distintas (Player vs Alien)
            if (a->getFaction() != b->getFaction())
            {
                if (a->getBounds().findIntersection(b->getBounds()))
                    resolveCollision(a, b);
            }
            // Caso 2: Misma facción (Player vs PowerUp)
            else
            {
                // Solo nos interesa colision de misma faccion si es Nave vs PowerUp
                bool aIsShip = (dynamic_cast<SpaceShip *>(a) != nullptr);
                bool bIsShip = (dynamic_cast<SpaceShip *>(b) != nullptr);
                bool aIsPU = (dynamic_cast<PowerUp *>(a) != nullptr);
                bool bIsPU = (dynamic_cast<PowerUp *>(b) != nullptr);

                if ((aIsShip && bIsPU) || (bIsShip && aIsPU))
                {
                    if (a->getBounds().findIntersection(b->getBounds()))
                        resolveCollision(a, b);
                }
            }
        }
    }
}

void CollisionManager::resolveCollision(Entity *a, Entity *b)
{
    // 1. Proyectiles
    // Intentamos identificar si alguno es un proyectil
    Projectile *proj = dynamic_cast<Projectile *>(a);
    Entity *target = b;

    if (!proj) // Si 'a' no era, probamos con 'b'
    {
        proj = dynamic_cast<Projectile *>(b);
        target = a;
    }

    if (proj)
    {
        // Los PowerUps no detienen las balas
        if (dynamic_cast<PowerUp *>(target))
            return;

        // Aplicar daño
        target->takeDamage((float)proj->getDamage());
        proj->destroy(); // La bala desaparece

        // Efecto visual de impacto
        if (onProjectileImpact)
        {
            onProjectileImpact(proj->getPosition(), proj->getType());
        }

        // Si matamos a un Alien, notifcamos (para puntos y drops)
        if (!target->isAlive() && target->getFaction() == Faction::Alien)
        {
            if (onEnemyDeath)
                onEnemyDeath(target->getPosition());
        }
        return;
    }

    // 2. El choque del player contra el enemigo
    // Daño por colision fisica
    if (a->getFaction() == Faction::Player && b->getFaction() == Faction::Alien)
    {
        a->takeDamage(50.f);  // Player recibe daño
        b->takeDamage(100.f); // Enemigo recibe mas daño (suele morir)

        if (!b->isAlive() && onEnemyDeath)
            onEnemyDeath(b->getPosition());
        return;
    }
    else if (b->getFaction() == Faction::Player && a->getFaction() == Faction::Alien)
    {
        b->takeDamage(50.f);
        a->takeDamage(100.f);

        if (!a->isAlive() && onEnemyDeath)
            onEnemyDeath(a->getPosition());
        return;
    }

    // 3. Para juntar los powerups
    SpaceShip *ship = dynamic_cast<SpaceShip *>(a);
    PowerUp *powerup = dynamic_cast<PowerUp *>(b);

    if (!ship)
    {
        ship = dynamic_cast<SpaceShip *>(b);
        powerup = dynamic_cast<PowerUp *>(a);
    }

    if (ship && powerup)
    {
        ship->applyPowerUp(powerup->getPowerUpType());
        powerup->destroy(); // Consumimos el item y desaparece
    }
}

void CollisionManager::setOnEnemyDeath(std::function<void(sf::Vector2f)> callback)
{
    onEnemyDeath = callback;
}

void CollisionManager::setOnProjectileImpact(std::function<void(sf::Vector2f, ProjectileType)> callback)
{
    onProjectileImpact = callback;
}