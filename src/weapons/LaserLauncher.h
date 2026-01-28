#pragma once
#include "Weapon.h"

class LaserLauncher : public Weapon
{
public:
    LaserLauncher(const std::string &name, float fireRate, float speed, int dmg)
        : Weapon(name, fireRate, speed, dmg) {}

    Projectile *Shoot(const sf::Vector2f &startPos) override;
};