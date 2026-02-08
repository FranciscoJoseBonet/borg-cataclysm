#pragma once
#include "Enemy.h"

class Scout : public Enemy
{
public:
    Scout(const sf::Texture &texture, sf::Vector2f startPos);
    void movePattern(float deltaTime) override;
};