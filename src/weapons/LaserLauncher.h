#pragma once
#include "Weapon.h"

class LaserLauncher : public Weapon
{
public:
    using Weapon::Weapon;
    void Shoot(const sf::Vector2f &startPos) override;
};