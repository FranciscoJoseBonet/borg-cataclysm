#pragma once

#include <SFML/Graphics.hpp>
#include "../Entity.h"

enum class ProjectileType
{
    LASER,
    MISSILE
};

class Projectile : public Entity
{
protected:
    // Estadisticas de combate
    sf::Vector2f direction;
    float speed;
    int damage;

    ProjectileType type;

    // Visuales
    sf::Sprite sprite;

    // Metodo interno para que la ventana nos dibuje el proy
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Projectile(sf::Vector2f direction, float speed, int damage, const sf::Texture &texture, Faction faction, ProjectileType type);

    virtual ~Projectile() = default;

    // Ciclo de vidadel proj
    void update(float deltaTime) override;

    // Gtt
    sf::FloatRect getBounds() const override;

    int getDamage() const { return damage; }
    ProjectileType getType() const { return type; }
};