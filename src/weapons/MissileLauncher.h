#pragma once
#include "Weapon.h"

class MissileLauncher : public Weapon
{
public:
    using Weapon::Weapon;
    void Shoot(const sf::Vector2f &startPos) override;
};