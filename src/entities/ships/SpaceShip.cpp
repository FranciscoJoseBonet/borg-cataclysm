#include "SpaceShip.h"
#include <iostream>
#include "../../weapons/LaserLauncher.h"
#include "../../weapons/MissileLauncher.h"

SpaceShip::SpaceShip()
    : Entity(100.f, "Federation", Faction::Player)
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

    sf::Vector2f pos = getPosition();

    float minX = 50.f;
    float maxX = 1230.f;
    float minY = 50.f;
    float maxY = 860.f;

    if (pos.x < minX)
        pos.x = minX;
    else if (pos.x > maxX)
        pos.x = maxX;
    if (pos.y < minY)
        pos.y = minY;
    else if (pos.y > maxY)
        pos.y = maxY;

    setPosition(pos);

    if (isInvulnerable)
    {
        invulnerabilityTimer -= deltaTime;

        blinkTimer += deltaTime;

        if (blinkTimer >= 0.1f)
        {
            blinkTimer = 0.f;
            isBlinkVisible = !isBlinkVisible;

            if (sprite)
            {
                if (isBlinkVisible)
                {
                    sprite->setColor(sf::Color(255, 255, 255, 128));
                }
                else
                {
                    sprite->setColor(sf::Color(255, 255, 255, 50));
                }
            }
        }

        if (invulnerabilityTimer <= 0.f)
        {
            isInvulnerable = false;
            if (sprite)
                sprite->setColor(sf::Color(255, 255, 255, 255));
        }
    }

    if (rapidFireTimer > 0.f)
    {
        rapidFireTimer -= deltaTime;
        if (rapidFireTimer <= 0.f)
        {
            if (laserLauncher)
                laserLauncher->setFireRate(5.f);
        }
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
    {
        states.transform *= getTransform();
        target.draw(*sprite, states);
    }
}

sf::FloatRect SpaceShip::getBounds() const
{
    if (sprite)
    {
        return getTransform().transformRect(sprite->getGlobalBounds());
    }
    return sf::FloatRect();
}

void SpaceShip::applyPowerUp(PowerUpType type)
{
    switch (type)
    {
    case PowerUpType::SHIELD:
        heal(30.f);
        break;
    case PowerUpType::DOUBLE_SHOT:
        enableDoubleShot();
        break;
    case PowerUpType::RAPID_FIRE:
        enableRapidFire();
        break;
    case PowerUpType::INVINCIBILITY:
        setInvulnerable(5.0f);
        break;
    }
}

void SpaceShip::heal(float amount)
{
    health += amount;
    if (health > 100.f)
        health = 100.f;
    std::cout << "Curado! Vida actual: " << health << "\n";
}

void SpaceShip::enableDoubleShot()
{
    doubleShotActive = true;
    std::cout << "¡DOBLE CAÑÓN ACTIVADO!\n";
}

void SpaceShip::enableRapidFire()
{
    if (laserLauncher)
    {
        laserLauncher->setFireRate(15.f);
        rapidFireTimer = 5.0f;
        std::cout << "Rapid Fire Activado!\n";
    }
}

void SpaceShip::setInvulnerable(float duration)
{
    isInvulnerable = true;
    invulnerabilityTimer = duration;

    blinkTimer = 0.f;
    isBlinkVisible = true;

    if (sprite)
        sprite->setColor(sf::Color(255, 255, 255, 128));

    std::cout << "¡Escudos Invencibles Activados!\n";
}

void SpaceShip::takeDamage(float amount)
{
    if (isInvulnerable)
        return;

    Entity::takeDamage(amount);
    doubleShotActive = false;

    std::cout << "Jugador danado! Vida: " << health << "\n";
}

void SpaceShip::addShield(float shieldAmount)
{
    heal(shieldAmount);
}