#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class StarField
{
private:
    struct Star
    {
        float speed;
    };

    sf::VertexArray vertices;
    std::vector<Star> starData;

    sf::Vector2u windowSize;
    std::mt19937 rng;

public:
    StarField(sf::Vector2u size, int count = 200);

    void update(float dt);
    void draw(sf::RenderWindow &window);
};