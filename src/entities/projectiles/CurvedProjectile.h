#pragma once
#include "Projectile.h"

class CurvedProjectile : public Projectile
{
private:
    float curveFrequency;
    float curveWidth;
    float timeAlive;
    float maxLifetime;

public:
    CurvedProjectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, int damage, float curveFrequency, const sf::Texture &texture, Faction faction);

    void update(float dt) override;
};