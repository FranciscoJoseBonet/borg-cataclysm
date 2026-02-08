#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>

class Explorer : public Enemy
{
private:
    float timeAlive = 0.f;

    float frequency;
    float amplitude;
    float startPhase;
    float initialX;

public:
    Explorer(const sf::Texture &texture, sf::Vector2f startPos);

    void update(float dt) override;
    void movePattern(float dt) override;
};