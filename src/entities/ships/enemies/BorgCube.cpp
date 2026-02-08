#include "BorgCube.h"
#include <cmath>
#include <iostream> // Para debug si hace falta

BorgCube::BorgCube(const sf::Texture &texture, sf::Vector2f startPos, int stage, float screenWidth)
    : Enemy(texture, 1000.f, "Borg Cube", 1.0f)
{
    this->bossStage = stage;
    this->screenWidth = screenWidth;
    this->hoverTime = 0.f;
    this->directionX = 1;

    setPosition(startPos);

    float targetWidthPercent = 0.15f;
    float hpMultiplier = 1.0f;
    float fireRateBase = 1.0f;

    if (bossStage == 1) // Nivel 3
    {
        targetWidthPercent = 0.15f;
        hpMultiplier = 1.0f;
        fireRateBase = 1.0f;
    }
    else if (bossStage == 2) // Nivel 6
    {
        targetWidthPercent = 0.25f;
        hpMultiplier = 2.5f;
        fireRateBase = 0.8f;
    }
    else // Nivel 9+
    {
        targetWidthPercent = 0.40f;
        hpMultiplier = 5.0f;
        fireRateBase = 0.5f;
    }

    // Aplicar atributos
    health = 1000.f * hpMultiplier;
    setFireRate(fireRateBase);

    if (sprite)
    {
        float textureWidth = sprite->getLocalBounds().size.x;
        float desiredWidth = screenWidth * targetWidthPercent;

        float scaleFactor = desiredWidth / textureWidth;

        sprite->setScale({scaleFactor, scaleFactor});
        auto bounds = sprite->getLocalBounds();
        sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    }
}

void BorgCube::update(float dt)
{
    Enemy::update(dt);
}

void BorgCube::movePattern(float dt)
{
    sf::Vector2f pos = getPosition();

    float halfWidth = 50.f;
    if (sprite)
        halfWidth = sprite->getGlobalBounds().size.x / 2.f;

    // ENTRADA TRIUNFAL
    if (pos.y < 200.f)
    {
        setPosition({pos.x, pos.y + (60.f * dt)});
    }
    // PATRULLAJE
    else
    {
        float speedX = 60.f;
        pos.x += speedX * directionX * dt;

        // REBOTE DINÁMICO

        if (pos.x > (screenWidth - halfWidth))
            directionX = -1;
        if (pos.x < halfWidth)
            directionX = 1;

        hoverTime += dt;
        float hoverY = std::sin(hoverTime * 2.f) * 0.5f;

        setPosition({pos.x, 200.f + hoverY});
    }
}