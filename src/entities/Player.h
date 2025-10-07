#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;

class Player
{
private:
    sf::Clock clock;
    float speed = 200.f;
    sf::Texture texture;
    unique_ptr<sf::Sprite> sprite;

public:
    Player();
    void draw(sf::RenderWindow &window);
    void update();
};
