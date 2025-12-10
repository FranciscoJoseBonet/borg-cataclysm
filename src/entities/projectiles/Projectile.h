#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"

class Projectile : public Entity
{
private:
    // Atts
    float speed;
    int damage;
    sf::Vector2f direction;
    sf::Texture texture;
    sf::Sprite sprite;

    // Meths
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    // Const
    Projectile(sf::Vector2f direction, float speed, int damage);

    // Meths
    void update(float deltaTime) override;

    int getDamage() { return damage; }
};