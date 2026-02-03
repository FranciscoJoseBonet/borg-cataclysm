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

        img.resize({20, 20}, sf::Color::Red);
    }

    if (texture.loadFromImage(img))
    {
        sprite.emplace(texture);
        auto bounds = sprite->getLocalBounds();
        sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    }

    setPosition({540.f, 840.f});
    if (sprite)
        sprite->setPosition(getPosition());

    laserLauncher = new LaserLauncher("Phaser Bank", 5.f, 600.f, 10);
    missileLauncher = new MissileLauncher("Photon Torpedo", 1.f, 100.f, 50);
}

SpaceShip::~SpaceShip()
{
    delete laserLauncher;
    delete missileLauncher;
}

void SpaceShip::destroy()
{
    Entity::destroy();
}

void SpaceShip::setWeaponsCallback(OnFireCallback callback)
{
    if (laserLauncher)
        laserLauncher->setCallback(callback);

    if (missileLauncher)
        missileLauncher->setCallback(callback);
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

    if (sprite)
    {
        sf::Vector2f pos = getPosition();
        auto bounds = sprite->getGlobalBounds();

        float minX = bounds.size.x / 2.f;
        float maxX = 1080.f - bounds.size.x / 2.f;
        float minY = bounds.size.y / 2.f;
        float maxY = 920.f - bounds.size.y / 2.f;

        if (pos.x < minX)
            pos.x = minX;
        else if (pos.x > maxX)
            pos.x = maxX;

        if (pos.y < minY)
            pos.y = minY;
        else if (pos.y > maxY)
            pos.y = maxY;

        setPosition(pos);
        sprite->setPosition(pos);
    }

    if (laserLauncher)
        laserLauncher->update(deltaTime);
    if (missileLauncher)
        missileLauncher->update(deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I) && laserLauncher)
    {
        laserLauncher->Shoot(getPosition());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O) && missileLauncher)
    {
        missileLauncher->Shoot(getPosition());
    }
}

void SpaceShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (sprite)
        target.draw(*sprite);
}