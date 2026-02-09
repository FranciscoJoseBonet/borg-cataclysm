#pragma once
#include <string>
#include <functional>
#include <SFML/System/Vector2.hpp>
#include "../entities/projectiles/Projectile.h"

// Callback que usamos para disparar: Tipo de bala, Posicion de inicio, Daño, Velocidad
using OnFireCallback = std::function<void(ProjectileType, const sf::Vector2f &, int, float)>;

class Weapon
{
protected:
    // Identificacion del arma que necesitamos
    std::string name;

    // Stats del disparo
    float fireRate;     // Disparos por segundo
    float cooldownTime; // Tiempo entre disparos (1 / fireRate)
    float currentCooldown;

    // Stats del proyectilll
    float projectileSpeed;
    int damage;

    OnFireCallback onFire; // Funcion a llamar cuando el arma se dispara exitosamente

public:
    Weapon(const std::string &name, float fireRate, float speed, int dmg);
    virtual ~Weapon() = default;

    // Ciclo del juego
    void update(float deltaTime);

    // Metodo Virtual: Las clases hijas (LaserLauncher, MissileLauncher u otras que agreguemos en el futuro)
    // pueden sobrescribir esto para cambiar QUE disparan o COMO disparan
    virtual void Shoot(const sf::Vector2f &startPos);

    // Stt
    void setCallback(OnFireCallback callback) { onFire = callback; }
    void setFireRate(float rate);
    void setProjectileSpeed(float speed) { projectileSpeed = speed; }

    // Gtt
    std::string getName() const { return name; }
};