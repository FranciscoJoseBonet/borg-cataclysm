#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../../weapons/Weapon.h"
#include "../projectiles/Projectile.h"
#include <vector>

class SpaceShip : public Entity
{
private:
    // Atts
    float speed = 200.f;
    sf::Texture texture;
    sf::Sprite sprite;
    Weapon *weapon;
    std::vector<Projectile *> projectiles;

    // Meths
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    // Const
    SpaceShip();

    // Meths
    void update(float deltaTime) override;
};