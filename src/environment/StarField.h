#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class StarField
{
private:
    // Estructura interna para guardar datos que el VertexArray no guarda
    struct StarData
    {
        float speed;
    };

    // VertexArray es un contenedor muy bueno para graficos primitivos (puntos, lineas)
    // Es muy util aca porque dibujar cientos de sf::Sprite individuales bajaria los FPS!!!!!
    // Con esto, dibujamos todo el fondo de una
    sf::VertexArray vertices;

    // Vector paralelo para guardar la velocidad de cada punto
    std::vector<StarData> stars;

    sf::Vector2u windowSize;

    // Generador de numeros aleatorios
    std::mt19937 rng;

public:
    StarField(sf::Vector2u size, int count = 200);

    void update(float dt);
    void draw(sf::RenderTarget &target) const;
};