#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../../weapons/Weapon.h"
#include "../projectiles/Projectile.h"
#include <vector>
#include <optional>

class SpaceShip : public Entity
{
private:
    float speed = 300.f;
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;

    Weapon *weapon = nullptr;
    Weapon *missileLauncher = nullptr;
    std::vector<Projectile *> projectiles;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    SpaceShip();

    void update(float deltaTime) override;
};
