#pragma once
#include <string>
#include <functional>
#include <SFML/System/Vector2.hpp>

enum class ProjectileType
{
    LASER,
    MISSILE
};

using OnFireCallback = std::function<void(ProjectileType, const sf::Vector2f &, int, float)>;

class Weapon
{
protected:
    std::string name;
    float fireRate;
    float cooldownTime;
    float projectileSpeed;
    int damage;
    float currentCooldown;
    OnFireCallback onFire;

public:
    Weapon(const std::string &name, float fireRate, float speed, int dmg);
    virtual ~Weapon() = default;

    void setCallback(OnFireCallback callback) { onFire = callback; }
    void setProjectileSpeed(float speed) { projectileSpeed = speed; }

    void setFireRate(float rate);

    virtual void Shoot(const sf::Vector2f &startPos);

    void update(float deltaTime);
    std::string getName() const { return name; }
};