#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>

// Enemigo que se mueve en patron de Onda Senoidal (Seno)
class Explorer : public Enemy
{
private:
    // Movimiento
    float timeAlive = 0.f; // Tiempo acumulado para calcular el Seno
    float initialX;        // Posicion X central sobre la que oscila

    // Parametros de la onda, despues los generamos random con el rng
    float frequency;  // Que tan rapido va de un lado a otro
    float amplitude;  // Que tan ancho es el movimiento
    float startPhase; // En que punto de la onda empieza

public:
    Explorer(const sf::Texture &texture, sf::Vector2f startPos);

    // Solo implementamos el movimiento
    // y usamos el update() base de Enemy
    void movePattern(float dt) override;
};