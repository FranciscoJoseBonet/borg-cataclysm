#include "Player.h"
#include <iostream>
using namespace std;

Player::Player()
{
    sf::Image img;
    if (!img.loadFromFile("../assets/img/enterprise-001.JPG"))
    {
        cerr << "No se pudo cargar la textura del jugador. Usando imagen vacía.\n";
        sf::Image img({20, 20}, sf::Color::Red);
    }

    if (!texture.loadFromImage(img))
        cerr << "No se pudo cargar textura desde imagen.\n";

    // Crear sprite asociado a la textura
    sprite = make_unique<sf::Sprite>(texture);
    sprite->setPosition({615.f, 500.f});
}

void Player::draw(sf::RenderWindow &window)
{
    if (sprite)
        window.draw(*sprite);
}
void Player::update()
{
    sf::Time dt = clock.restart();

    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        movement.x -= speed * dt.asSeconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        movement.x += speed * dt.asSeconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        movement.y -= speed * dt.asSeconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        movement.y += speed * dt.asSeconds();

    sprite->move(movement);
}
