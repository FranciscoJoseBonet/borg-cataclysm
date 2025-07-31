#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics/Drawable.hpp>

class Entity : public sf::Drawable
{
public:
    virtual void update(float deltaTime) = 0;

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};

#endif
