#pragma once
#include <SFML/Graphics.hpp>

// Entity AHORA ES un Transformable.
// Tiene acceso público a setPosition, move, etc.
class Entity : public sf::Drawable, public sf::Transformable
{
public:
    virtual ~Entity() = default;
    virtual void update(float deltaTime) = 0;

protected:
    // Al dibujar, usaremos getTransform() heredado de Transformable
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};