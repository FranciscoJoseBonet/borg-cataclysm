#pragma once
#include "Projectile.h"

class MissileProjectile : public Projectile
{
private:
    float maxSpeed;
    float acceleration;

public:
    MissileProjectile(sf::Vector2f direction, float initialSpeed, float maxSpeed, int damage, const sf::Texture &texture, Faction faction);

    void update(float deltaTime) override;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void destroy() override;
};