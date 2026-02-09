#pragma once
#include "Weapon.h"

class LaserLauncher : public Weapon
{
public:
    using Weapon::Weapon;

    // Sobrescribimos para lanzar laser
    void Shoot(const sf::Vector2f &startPos) override;
};