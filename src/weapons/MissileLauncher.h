#pragma once
#include "Weapon.h"
#include "../entities/projectiles/MissileProjectile.h"

class MissileLauncher : public Weapon
{
public:
    using Weapon::Weapon;

    Projectile *Shoot(const sf::Vector2f &startPos) override;
};