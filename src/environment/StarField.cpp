#include "StarField.h"
#include <cstdint>

StarField::StarField(sf::Vector2u size, int count)
    : windowSize(size), rng(std::random_device{}())
{
    vertices.setPrimitiveType(sf::PrimitiveType::Points);
    vertices.resize(count);
    starData.resize(count);

    std::uniform_real_distribution<float> distX(0.f, (float)size.x);
    std::uniform_real_distribution<float> distY(0.f, (float)size.y);
    std::uniform_real_distribution<float> distSpeed(20.f, 150.f);

    for (int i = 0; i < count; ++i)
    {
        float x = distX(rng);
        float y = distY(rng);
        vertices[i].position = {x, y};
        float speed = distSpeed(rng);
        starData[i].speed = speed;

        std::uint8_t alpha = static_cast<std::uint8_t>((speed / 150.f) * 255);

        vertices[i].color = sf::Color(200, 200, 255, alpha);
    }
}

void StarField::update(float dt)
{
    std::uniform_real_distribution<float> distX(0.f, (float)windowSize.x);

    for (size_t i = 0; i < vertices.getVertexCount(); ++i)
    {
        vertices[i].position.y += starData[i].speed * dt;

        if (vertices[i].position.y > windowSize.y)
        {
            vertices[i].position.y = 0.f;
            vertices[i].position.x = distX(rng);
        }
    }
}

void StarField::draw(sf::RenderWindow &window)
{
    window.draw(vertices);
}