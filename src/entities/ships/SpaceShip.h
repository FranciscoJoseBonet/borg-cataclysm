#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../../weapons/Weapon.h"
#include <optional>
#include <memory>

class SpaceShip : public Entity
{
private:
    float speed = 300.f;
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;

    Weapon *laserLauncher = nullptr;
    Weapon *missileLauncher = nullptr;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    SpaceShip();
    ~SpaceShip();

    void setWeaponsCallback(OnFireCallback callback);
    sf::FloatRect getBounds() const override;
    void destroy() override;
    void update(float deltaTime) override;
};