#include "SpaceShip.h"
#include <iostream>

#include <vector>

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

    // Posicion inicial
    sprite.setPosition({615.f, 500.f});
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

    // Para disparar (crea el disparo)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I))
    {
        Projectile *newShot = weapon->Shoot(sprite.getPosition());
        projectiles.push_back(newShot);
    }

    // Actualizamos todos los proyectiles
    for (auto *p : projectiles)
        p->update(deltaTime);
}

// Esta funcion actualiza el dibujo de la entidad acorde a sus transformaciones
void SpaceShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(sprite, states);

    for (auto *p : projectiles)
        target.draw(*p);
}
