#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Player : public Entity
{
private:
    sf::Clock clock;
    float speed = 200.f;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Player();
    void update(float deltaTime) override;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};