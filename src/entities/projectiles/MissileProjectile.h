#pragma once
#include "Projectile.h"

class MissileProjectile : public Projectile
{
private:
    float maxSpeed;
    float acceleration;
    float currentSpeed; // Variable interna para rastrear la velocidad actual

public:
    MissileProjectile(sf::Vector2f direction, float initialSpeed, float maxSpeed, int damage, const sf::Texture &texture, Faction faction);

    void update(float deltaTime) override;
};