#include "PowerUp.h"

PowerUp::PowerUp(sf::Vector2f position, PowerUpType type, const sf::Texture &texture)
    : Entity(1.f, "PowerUp", Faction::Neutral),
      powerUpType(type),
      speed(100.f),
      sprite(texture)
{
    setPosition(position);

    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    sprite.setScale({0.3f, 0.3f});
}

void PowerUp::update(float deltaTime)
{
    // Movimiento vertical descendente simple
    move({0.f, speed * deltaTime});

    // Si sale de la pantalla por abajo, se destruye (si pasa los 1300)
    if (getPosition().y > 1300.f)
    {
        destroy();
    }
}

sf::FloatRect PowerUp::getBounds() const
{
    // Obtenemos la hb transformada (con posicion y escala aplicadas)
    return getTransform().transformRect(sprite.getGlobalBounds());
}

void PowerUp::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Aplicamos las transformaciones de la entidad (posicion X,Y) al estado de render
    states.transform *= getTransform();
    target.draw(sprite, states);
}