#include "SpaceShip.h"
#include <iostream>
#include "../../weapons/LaserLauncher.h"
#include "../../weapons/MissileLauncher.h"

SpaceShip::SpaceShip()
    : Entity(100.f, "Federation")
{
    sf::Image img;
    if (!img.loadFromFile("../assets/img/enterprise-001.PNG"))
    {
        std::cerr << "No se pudo cargar la textura del jugador\n";
        img = sf::Image({20, 20}, sf::Color::Red);
    }

    if (texture.loadFromImage(img))
    {
        sprite.emplace(texture);

        auto bounds = sprite->getLocalBounds();
        sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    }

    // Posición inicial visible
    setPosition({540.f, 840.f});
    if (sprite)
        sprite->setPosition(getPosition());

    weapon = new LaserLauncher("Phaser Bank", 5.f, 600.f, 10);
    missileLauncher = new MissileLauncher("Photon Torpedo", 1.f, 100.f, 50);
}

void SpaceShip::update(float deltaTime)
{
    // --- MOVIMIENTO ---
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed * deltaTime;

    move(movement);

    // --- LÍMITES DE PANTALLA ---
    if (sprite)
    {
        sf::Vector2f pos = getPosition();
        auto bounds = sprite->getGlobalBounds();

        if (pos.x < bounds.size.x / 2.f)
            pos.x = bounds.size.x / 2.f;
        else if (pos.x > 1080.f - bounds.size.x / 2.f)
            pos.x = 1080.f - bounds.size.x / 2.f;

        if (pos.y < bounds.size.y / 2.f)
            pos.y = bounds.size.y / 2.f;
        else if (pos.y > 920.f - bounds.size.y / 2.f)
            pos.y = 920.f - bounds.size.y / 2.f;

        setPosition(pos);
        sprite->setPosition(pos);
    }

    // --- ARMAS ---
    if (weapon)
        weapon->update(deltaTime);
    if (missileLauncher)
        missileLauncher->update(deltaTime);

    // Láser (I)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I) && weapon)
    {
        if (Projectile *shot = weapon->Shoot(getPosition()))
            projectiles.push_back(shot);
    }

    // Misil (O)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O) && missileLauncher)
    {
        if (Projectile *missile = missileLauncher->Shoot(getPosition()))
            projectiles.push_back(missile);
    }

    // --- PROYECTILES ---
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        (*it)->update(deltaTime);

        if ((*it)->getPosition().y < -50.f)
        {
            delete *it;
            it = projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void SpaceShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (sprite)
        target.draw(*sprite);

    for (auto *p : projectiles)
        target.draw(*p);
}
