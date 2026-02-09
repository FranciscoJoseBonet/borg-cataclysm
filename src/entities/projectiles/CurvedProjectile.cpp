#include "CurvedProjectile.h"
#include <cmath>

CurvedProjectile::CurvedProjectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, int damage, float curveFrequency, const sf::Texture &texture, Faction faction)
    : Projectile(direction, speed, damage, texture, faction, ProjectileType::LASER),
      curveFrequency(curveFrequency),
      curveWidth(3.0f),
      timeAlive(0.f)
{
    setPosition(startPos);
}

void CurvedProjectile::update(float dt)
{
    timeAlive += dt;

    // Calculamos el desplazamiento lateral, es la parte de la onda,
    // modifica el vector de movimiento original sumandole un valor oscilante a X
    float sideMovement = std::cos(timeAlive * curveFrequency) * curveWidth;

    sf::Vector2f moveVector = direction;
    moveVector.x += sideMovement;

    move(moveVector * speed * dt);

    // Rotacion visual dinamica, ahora el asset es circular pero si ponemos un asset con otra forma queda bien el efecto
    float rotation = std::atan2(moveVector.y, moveVector.x) * 180.f / 3.14159265f;
    sprite.setRotation(sf::degrees(rotation + 90.f));

    // Limpieza por salir de pantalla
    sf::Vector2f pos = getPosition();
    if (pos.y > 1500.f || pos.y < -500.f || pos.x < -500.f || pos.x > 2000.f)
    {
        destroy();
    }
}