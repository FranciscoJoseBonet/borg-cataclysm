#include "Weapon.h"

Weapon::Weapon(const std::string &name, float fireRate, float speed, int dmg)
    : name(name), fireRate(fireRate), projectileSpeed(speed), damage(dmg), currentCooldown(0.f)
{
    setFireRate(fireRate);
}

void Weapon::setFireRate(float rate)
{
    this->fireRate = rate;

    if (rate > 0)
    {
        this->cooldownTime = 1.f / rate;
    }
    else
    {
        this->cooldownTime = 0.f;
    }
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