#include "SpaceShip.h"
#include <iostream>
#include <vector>
#include "../../core/Game.h"

SpaceShip::SpaceShip() : Entity(100.f, "Federation"), sprite(texture)
{
    sf::Image img;

    if (!img.loadFromFile("../assets/img/enterprise-001.PNG"))
    {
        std::cerr << "No se pudo cargar la textura del jugador\n";
        img = sf::Image({20, 20}, sf::Color::Red); // Para evitar error por la carga de la imagen
    }

    if (!texture.loadFromImage(img))
        std::cerr << "No se pudo cargar textura desde imagen\n";

    sprite = sf::Sprite(texture);

    this->weapon = new Weapon("Phaser", 5.f, 600.f, 10);
}

// Esta funcion actualiza la entidad (no grafico)
void SpaceShip::update(float deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);

    // Lo multiplico por el delta tiempo para que la velocidad sea uniforme e independiente de los fps
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed * deltaTime;

    move(movement);

    if (weapon)
    {
        weapon->update(deltaTime);
    }
    // crea el disparo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I) && weapon)
    {
        Projectile *newShot = weapon->Shoot(getPosition());

        if (newShot != nullptr)
        {
            projectiles.push_back(newShot);
        }
    }

    for (auto *p : projectiles)
    {
        if (p)
            p->update(deltaTime);
    }
}

// Esta funcion actualiza el dibujo de la entidad acorde a sus transformaciones
void SpaceShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(sprite, states);

    for (auto *p : projectiles)
        target.draw(*p);
}
