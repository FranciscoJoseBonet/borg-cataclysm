#include "SpaceShip.h"
#include <iostream>

SpaceShip::SpaceShip() : Entity(100.f, "Federation"), sprite(texture)
{
    sf::Image img;

    if (!img.loadFromFile("../assets/img/enterprise-001.JPG"))
    {
        std::cerr << "No se pudo cargar la textura del jugador. Usando imagen temporal.\n";
        img = sf::Image({20, 20}, sf::Color::Red);
    }

    if (!texture.loadFromImage(img))
        std::cerr << "No se pudo cargar textura desde imagen.\n";

    sprite = sf::Sprite(texture);
    sprite.setPosition({615.f, 500.f});
}

void SpaceShip::update(float deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed * deltaTime;

    sprite.move(movement);
}

void SpaceShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
