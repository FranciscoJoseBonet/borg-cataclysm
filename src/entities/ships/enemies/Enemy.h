#pragma once
#include "../../Entity.h"
#include "../../../weapons/Weapon.h"

class Enemy : public Entity
{
protected:
    Weapon *weapon = nullptr;
    float fireTimer;
    float fireInterval;

    OnFireCallback onFire;

public:
    Enemy(float health, const std::string &type, float fireRate);
    virtual ~Enemy();

    void setFireCallback(OnFireCallback callback);
    void setFireRate(float rate);
    void update(float deltaTime) override;
    void setProjectileSpeed(float speed);

    virtual void movePattern(float deltaTime) = 0;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};