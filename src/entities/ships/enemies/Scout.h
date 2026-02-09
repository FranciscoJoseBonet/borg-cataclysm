#pragma once
#include "Enemy.h"

// Enemigo basico: Se mueve recto hacia abajo
class Scout : public Enemy
{
public:
    // Constructor especifico
    Scout(const sf::Texture &texture, sf::Vector2f startPos);

    // Implementacion del movimiento recto
    void movePattern(float deltaTime) override;
};