#pragma once
#include "Enemy.h"

class Scout : public Enemy
{
private:
    float speed;
    sf::Sprite sprite;

public:
    Scout(const sf::Texture &texture, sf::Vector2f startPos);
    void movePattern(float deltaTime) override;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};