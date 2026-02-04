#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class Faction
{
    Player,
    Alien,
    Neutral
};

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity(float startHealth, const std::string &type, Faction faction)
        : health(startHealth), type(type), faction(faction), alive(true) {}

    virtual ~Entity() = default;

    virtual void update(float deltaTime) = 0;

    virtual void takeDamage(float amount)
    {
        health -= amount;
        if (health <= 0 && alive)
            destroy();
    }
    float getHealth() const { return health; }

    void setHealth(float h) { health = h; }

    bool isAlive() const { return alive; }

    Faction getFaction() const { return faction; }

    virtual sf::FloatRect getBounds() const = 0;

    virtual void destroy() { alive = false; }
    std::string getType() const { return type; }

protected:
    float health;
    std::string type;
    Faction faction;
    bool alive;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};