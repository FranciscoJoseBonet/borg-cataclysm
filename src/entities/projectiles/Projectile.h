#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"

class Projectile : public Entity
{
private:
    float speed;
    int damage;
    sf::Vector2f direction;
    sf::Texture texture;
    sf::Sprite sprite;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Projectile(sf::Vector2f direction, float speed, int damage);

    void update(float deltaTime) override;
    int getDamage() { return damage; }
};