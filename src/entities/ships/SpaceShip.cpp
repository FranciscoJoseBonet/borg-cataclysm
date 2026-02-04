#include "SpaceShip.h"
#include <iostream>
#include <algorithm>
#include "../../weapons/LaserLauncher.h"
#include "../../weapons/MissileLauncher.h"

SpaceShip::SpaceShip()
    : Entity(100.f, "Federation", Faction::Player),
      shieldSprite(shieldTexture)
{
    maxShield = 100.f;
    shield = 100.f;

    if (!shieldTexture.loadFromFile("../assets/img/shield.png"))
    {
        std::cerr << "ERROR: No se encontro shield.png - Usando textura de emergencia.\n";

        sf::Image temp;
        temp.resize({100, 100}, sf::Color::White);

        if (!shieldTexture.loadFromImage(temp))
        {
            std::cerr << "Error fatal creando textura temporal.\n";
        }
    }

    shieldSprite.setTexture(shieldTexture, true);

    auto sBounds = shieldSprite.getLocalBounds();
    shieldSprite.setOrigin({sBounds.size.x / 2.f, sBounds.size.y / 2.f});

    shieldSprite.setColor(sf::Color(0, 255, 255, 80));

    sf::Image img;
    if (!img.loadFromFile("../assets/img/enterprise-001.PNG"))
    {
        std::cerr << "ERROR: No se cargo la nave.\n";
        img.resize({20, 20}, sf::Color::Red);
    }

    if (texture.loadFromImage(img))
    {
        sprite.emplace(texture);
        auto shipBounds = sprite->getLocalBounds();

        sprite->setOrigin({shipBounds.size.x / 2.f, shipBounds.size.y / 2.f});

        auto shieldBounds = shieldSprite.getLocalBounds();

        float shipRadius = std::max(shipBounds.size.x, shipBounds.size.y) / 2.f;
        float shieldRadius = std::max(shieldBounds.size.x, shieldBounds.size.y) / 2.f;

        if (shieldRadius < 1.f)
            shieldRadius = 1.f;

        float scaleFactor = (shipRadius + 5.f) / shieldRadius;

        shieldSprite.setScale({scaleFactor, scaleFactor});
        shieldSprite.setPosition({0.f, -5.f});
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

    float minX = 30.f;
    float maxX = 1055.f;
    float minY = 40.f;
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
                sf::Color c = sprite->getColor();
                c.a = isBlinkVisible ? 128 : 50;
                sprite->setColor(c);
            }
        }

        if (invulnerabilityTimer <= 0.f)
        {
            isInvulnerable = false;
            if (sprite)
                sprite->setColor(sf::Color::White);
            std::cout << "Invulnerabilidad terminada.\n";
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
        laserLauncher->Shoot(getPosition());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O) && missileLauncher)
        missileLauncher->Shoot(getPosition());
}

void SpaceShip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    if (sprite)
        target.draw(*sprite, states);

    if (shield > 0)
        target.draw(shieldSprite, states);
}

sf::FloatRect SpaceShip::getBounds() const
{
    if (sprite)
        return getTransform().transformRect(sprite->getGlobalBounds());
    return sf::FloatRect();
}

void SpaceShip::applyPowerUp(PowerUpType type)
{
    switch (type)
    {
    case PowerUpType::SHIELD:
        addShield(50.f);
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
    case PowerUpType::HEAL:
        heal(100.f);
        break;
    case PowerUpType::EXTRA_LIFE:
        lives++;
        break;
    }
}

void SpaceShip::addShield(float amount)
{
    shield += amount;
    if (shield > maxShield)
        shield = maxShield;

    std::cout << "Escudo regenerado! Nivel actual: " << shield << "\n";
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
    std::cout << "¡DOBLE CANION ACTIVADO!\n";
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

    if (shield > 0)
    {
        float damageToShield = amount;

        if (damageToShield > shield)
        {
            float damageRemaining = damageToShield - shield;
            shield = 0;
            std::cout << "¡ESCUDO ROTO!\n";
            health -= damageRemaining;
        }
        else
        {
            shield -= damageToShield;
            std::cout << "Escudo absorbe impacto. Restante: " << shield << "\n";
            return;
        }
    }
    else
    {
        health -= amount;
    }

    std::cout << "Jugador danado! Salud: " << health << "\n";

    if (health <= 0.f)
    {
        lives--;
        if (lives > 0)
        {
            std::cout << "¡Nave Destruida! Reapareciendo... (Vidas restantes: " << lives << ")\n";
            respawn();
        }
        else
        {
            std::cout << "=== GAME OVER ===\n";
            Entity::destroy();
        }
    }
}

void SpaceShip::respawn()
{
    health = 100.f;

    shield = 30.f;

    setPosition({540.f, 840.f});

    if (sprite)
    {
        sprite->setPosition({0.f, 0.f});
    }

    setInvulnerable(3.0f);
    doubleShotActive = false;
    rapidFireTimer = 0.f;
    if (laserLauncher)
    {
        laserLauncher->setFireRate(5.f);
    }
}