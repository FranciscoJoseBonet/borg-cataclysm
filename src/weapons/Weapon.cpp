#include "Weapon.h"

Weapon::Weapon(const std::string &name, float fireRate, float speed, int dmg)
    : name(name), projectileSpeed(speed), damage(dmg), currentCooldown(0.f)
{
    this->cooldownTime = (fireRate > 0) ? 1.f / fireRate : 0.f;
}

void Weapon::update(float deltaTime)
{
    if (currentCooldown > 0.f)
        currentCooldown -= deltaTime;
}

void Weapon::Shoot(const sf::Vector2f &startPos)
{
    if (currentCooldown > 0.f)
        return;
    currentCooldown = cooldownTime;

    if (onFire)
    {
        onFire(ProjectileType::LASER, startPos, damage, projectileSpeed);
    }
}