#pragma once
#include "Enemy.h"

class Explorer : public Enemy
{
private:
    float timeAlive = 0.f;
    float frequency = 2.0f;
    float amplitude = 120.f;
    float initialX;

public:
    Explorer(const sf::Texture &texture, sf::Vector2f startPos);

    void update(float dt) override;
    void movePattern(float dt) override;
};