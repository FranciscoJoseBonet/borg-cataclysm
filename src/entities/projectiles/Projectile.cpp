#include "Projectile.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f direction, float speed, int damage, const sf::Texture &texture, Faction faction, ProjectileType type)
    : Entity(1, "Projectile", faction), // Vida 1 (se destruye al impacto)
      direction(direction),
      speed(speed),
      damage(damage),
      type(type),
      sprite(texture)
{
    // Configuracion del origen para rotaciones y colisiones mas precisas
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    // Esta orientacion simple es para que cualquier proyectil que se dispare mas adelante sea siempre en la direccion de direction

    // Calculamos la rotación basada en la dirección de movimiento
    // std::atan2 devuelve el ángulo en radianes
    // Multiplicamos por (180 / PI) para pasarlo a grados
    float angleRad = std::atan2(direction.y, direction.x);
    float angleDeg = angleRad * 180.f / 3.14159265f;

    // Aplicamos la rotación con un ajuste
    sprite.setRotation(sf::degrees(angleDeg));
}

void Projectile::update(float deltaTime)
{
    // Movimiento vectorial simple, direccion * velocidad * tiempo
    move(direction * speed * deltaTime);
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Aplicamos transformacion
    states.transform *= getTransform();
    target.draw(sprite, states);
}

sf::FloatRect Projectile::getBounds() const
{
    // Devolvemos la caja de colision transformada al mundo (nos dice las coords de la ventana en las que está el sprite)
    return getTransform().transformRect(sprite.getGlobalBounds());
}