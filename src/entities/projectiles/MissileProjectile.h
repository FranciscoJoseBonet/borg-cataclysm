#pragma once
#include "Projectile.h"

class MissileProjectile : public Projectile
{
private:
    float maxSpeed;
    float acceleration;

public:
    MissileProjectile(sf::Vector2f direction, float initialSpeed, float maxSpeed, int damage, const sf::Texture &texture);
    void update(float deltaTime) override;
    void destroy() override;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
