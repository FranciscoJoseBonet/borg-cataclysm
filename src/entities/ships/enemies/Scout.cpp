#include "Scout.h"

Scout::Scout(const sf::Texture &texture, sf::Vector2f startPos)
    : Enemy(texture, 30.f, "Scout", 1.0f) // Stats base: 30 HP, 1 disparo/seg
{
    // Configuracion especifica del Scout
    setSpeed(150.f);
    setPosition(startPos);

    // Como 'sprite' es protected en Enemy, podemos modificarlo aca
    sprite.setScale({0.6f, 0.6f});

    // Lo rotamos porque el sprite esta rotado (habría que normalizar todos los assets de enemies)
    sprite.setRotation(sf::degrees(90.f));
}

void Scout::movePattern(float deltaTime)
{
    // Movimiento lineal simple hacia abajo (Y positivo)
    move({0.f, getSpeed() * deltaTime});
}