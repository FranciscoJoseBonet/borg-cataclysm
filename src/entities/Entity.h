#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity(float startHealth, const std::string &type) : health(startHealth), type(type), alive(true) {}
    virtual ~Entity() = default;

    virtual void update(float deltaTime) = 0;

    virtual void takeDamage(float amount)
    {
        health -= amount;
        if (health <= 0)
            alive = false;
    }

    bool isAlive() const { return alive; }
    void destroy() { alive = false; }
    std::string getType() const { return type; }

protected:
    float health;
    bool alive;
    std::string type;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};