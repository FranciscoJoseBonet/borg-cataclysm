#include "StarField.h"
#include <cstdint>

StarField::StarField(sf::Vector2u size, int count)
    : windowSize(size),
      rng(std::random_device{}()) // Seed random
{
    // Configuramos el VertexArray para dibujar PUNTOS
    vertices.setPrimitiveType(sf::PrimitiveType::Points);
    vertices.resize(count); // Reservamos memoria grafica
    stars.resize(count);    // Reservamos memoria logica

    // Distribuciones para generar posiciones y velocidades iniciales
    std::uniform_real_distribution<float> distX(0.f, (float)size.x);
    std::uniform_real_distribution<float> distY(0.f, (float)size.y);
    std::uniform_real_distribution<float> distSpeed(20.f, 150.f); // velocidades de las estrellas entre 20 y 150

    for (int i = 0; i < count; ++i)
    {
        // 1. Posicion Inicial
        float x = distX(rng);
        float y = distY(rng);
        vertices[i].position = {x, y};

        // 2. Velocidad Aleatoria
        float speed = distSpeed(rng);
        stars[i].speed = speed;

        // 3. Color y Transparencia, este da el efecto de paralaje como el fondo de terraria
        // o el de la serie de startrek TNG :)
        // Config de las estrellas:
        // mas rapido = mas brillante (cerca) y mas lento = mas oscuro (lejos)
        std::uint8_t alpha = static_cast<std::uint8_t>((speed / 150.f) * 255); // El casteo aca es para que el color reciba un valor entero aprox,
                                                                               // no necesitamos exactitud solo que sea compatible el dato

        vertices[i].color = sf::Color(200, 200, 255, alpha); // Un tono azulcito
    }
}

void StarField::update(float dt)
{
    // Distribuciones para cuando la estrella se recicla
    std::uniform_real_distribution<float> distX(0.f, (float)windowSize.x);
    std::uniform_real_distribution<float> distSpeed(20.f, 150.f);

    for (std::size_t i = 0; i < vertices.getVertexCount(); ++i)
    {
        // Movemos la estrella en Y segun su velocidad individual
        vertices[i].position.y += stars[i].speed * dt;

        // Si sale de la pantalla por abajo
        if (vertices[i].position.y > windowSize.y)
        {
            // la mandamos arriba de nuevo
            vertices[i].position.y = 0.f;
            vertices[i].position.x = distX(rng); // Nueva posicion x aleatoria

            // Le damos una nueva velocidad para que no sea siempre igual
            float newSpeed = distSpeed(rng);
            stars[i].speed = newSpeed;

            // Recalculamos el brillo basado en la nueva velocidad
            std::uint8_t alpha = static_cast<std::uint8_t>((newSpeed / 150.f) * 255);
            vertices[i].color = sf::Color(200, 200, 255, alpha);
        }
    }
}

void StarField::draw(sf::RenderTarget &target) const
{
    // Una sola llamada a la GPU dibuja todas las estrellas despues reciclamos los puntitos
    target.draw(vertices);
}