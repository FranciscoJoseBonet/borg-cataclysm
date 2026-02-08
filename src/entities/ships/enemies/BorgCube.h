#pragma once
#include "Enemy.h"
#include <vector>

class BorgCube : public Enemy
{
private:
    int bossStage;
    float hoverTime;
    int directionX;

    float screenWidth;

public:
    BorgCube(const sf::Texture &texture, sf::Vector2f startPos, int bossStage, float screenWidth);

    void update(float dt) override;
    void movePattern(float dt) override;
};