#pragma once
#include "Enemy.h"
#include <vector>

class BorgCube : public Enemy
{
private:
    // Estado del Boss
    int bossStage;     // Nivel de dificultad (determina tamaño y vida)
    float screenWidth; // Necesario para saber donde rebotar

    // MOvimiento
    float hoverTime; // Acumulador para la funcion Seno
    int directionX;  // 1 o -1

public:
    BorgCube(const sf::Texture &texture, sf::Vector2f startPos, int bossStage, float screenWidth);

    // Solo necesitamos definir el patron de movimiento
    // porque el update general lo maneja la clase Enemy
    void movePattern(float dt) override;
};