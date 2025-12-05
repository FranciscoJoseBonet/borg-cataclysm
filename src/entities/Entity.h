#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Entity : public sf::Drawable
{
public:
    virtual ~Entity() = default;
    virtual void update(float deltaTime) = 0;

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};
