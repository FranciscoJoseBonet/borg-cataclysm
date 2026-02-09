#include "BorgCube.h"
#include <cmath>
#include <iostream>

BorgCube::BorgCube(const sf::Texture &texture, sf::Vector2f startPos, int stage, float sWidth)
    : Enemy(texture, 1000.f, "Borg Cube", 1.0f), // Vida base alta
      bossStage(stage),
      screenWidth(sWidth),
      hoverTime(0.f),
      directionX(1)
{
    setPosition(startPos);

    // 1. Config de dificultad (Segun el nivel actual)
    float targetWidthPercent = 0.15f;
    float hpMultiplier = 1.0f;
    float fireRateBase = 1.0f;

    if (bossStage == 1) // Primer pvp
    {
        targetWidthPercent = 0.15f; // Pequeño
        hpMultiplier = 1.0f;
        fireRateBase = 1.0f;
    }
    else if (bossStage == 2) // Segundo pvp
    {
        targetWidthPercent = 0.25f;
        hpMultiplier = 2.5f;
        fireRateBase = 0.8f;
    }
    else // Pvps finales
    {
        targetWidthPercent = 0.40f; // Gigante
        hpMultiplier = 5.0f;
        fireRateBase = 0.5f; // Ametralladora
    }

    // Aplicar los atributos calculados
    setHealth(1000.f * hpMultiplier);
    setFireRate(fireRateBase);

    // 2. Config visual
    // Esto es para escalar el boss a la pantalla
    float textureWidth = sprite.getLocalBounds().size.x;
    float desiredWidth = screenWidth * targetWidthPercent;
    float scaleFactor = desiredWidth / textureWidth;

    sprite.setScale({scaleFactor, scaleFactor});

    // Centramos el origen para que las balas salgan del centro y rote bien (Change prox act *****)
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
}

void BorgCube::movePattern(float dt)
{
    sf::Vector2f pos = getPosition();

    // Obtenemos el ancho real actual
    float halfWidth = sprite.getGlobalBounds().size.x / 2.f;

    // FASE 1: Baja lento para imponer miedo borg hasta la pos Y=200
    if (pos.y < 200.f)
    {
        setPosition({pos.x, pos.y + (60.f * dt)});
    }
    // FASE 2: Patrulla por la pantalla
    else
    {
        // Movimiento Horizontal
        float speedX = 75.f;
        pos.x += speedX * directionX * dt;

        // Rebota en los bordes de la pantalla
        if (pos.x > (screenWidth - halfWidth))
        {
            pos.x = screenWidth - halfWidth;
            directionX = -1; // Ir izquierda
        }
        else if (pos.x < halfWidth)
        {
            pos.x = halfWidth;
            directionX = 1; // Ir derecha
        }

        // Usamos una funcion Seno para que flote suavemente arriba y abajo
        hoverTime += dt;
        float hoverY = std::sin(hoverTime * 2.f) * 0.5f;

        // Fijamos la Y base en 200 teniendo en cuenta la oscilacion
        setPosition({pos.x, 200.f + hoverY});
    }
}