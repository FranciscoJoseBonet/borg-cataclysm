#pragma once
#include "Projectile.h"

// Proyectil que se mueve en patron de onda
class CurvedProjectile : public Projectile
{
private:
    float curveFrequency;
    float curveWidth;

    // Variable de ciclo de vida interno
    float timeAlive;

public:
    CurvedProjectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, int damage, float curveFrequency, const sf::Texture &texture, Faction faction);

    void update(float dt) override;
};