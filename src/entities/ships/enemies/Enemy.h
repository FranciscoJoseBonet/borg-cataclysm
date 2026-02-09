#pragma once
#include "../../Entity.h"
#include "../../projectiles/Projectile.h"
#include <SFML/Graphics.hpp>
#include <functional>

// Definimos el tipo de funcion para disparar
using OnFireCallback = std::function<void(ProjectileType, const sf::Vector2f &, int, float)>; // Callback

class Enemy : public Entity
{
protected:
    // Estadísticas de combate
    float speed;
    int damage; // Daño por colision

    // Sistema de disparo
    float fireTimer;
    float fireInterval; // Cadencia (tiempo entre los tiros)
    float projectileSpeed;

    OnFireCallback onFire; // Funcion que ejecuta el LevelManager para crear la bala

    // Visuales
    sf::Sprite sprite; // Sprite directo

    // Metodo interno para renderizar
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Enemy(const sf::Texture &texture, float health, const std::string &type, float fireRate);
    virtual ~Enemy() = default;

    // Ciclo de vIda
    void update(float deltaTime) override;

    // Como metodo virtual puro porque cada enemigo DEBE definir su patron de movimiento
    virtual void movePattern(float deltaTime) = 0;

    // setters
    void setFireCallback(OnFireCallback callback);
    void setFireRate(float rate);
    void setProjectileSpeed(float s);
    void setSpeed(float s) { speed = s; }

    // getters
    float getSpeed() const { return speed; }
    sf::FloatRect getBounds() const override;
};