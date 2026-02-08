#include "SpaceShip.h"
#include <iostream>
#include <algorithm>
#include "../../weapons/LaserLauncher.h"
#include "../../weapons/MissileLauncher.h"

SpaceShip::SpaceShip(const ShipData &shipData, const sf::Texture &texture)
    : Entity(shipData.maxHealth, shipData.name, Faction::Player),
      data(shipData),
      worldBounds({1280.f, 720.f}),
      shieldSprite(shieldTexture)
{
    maxShield = data.maxHealth;
    shield = maxShield;

    if (!shieldTexture.loadFromFile(data.shieldTexturePath))
    {
        std::cerr << "Error no se encontro la imagen del escudo: " << data.shieldTexturePath << "\n";
        sf::Image temp;
        temp.resize({100, 100}, sf::Color::White);
        if (!shieldTexture.loadFromImage(temp))
        {
        }
    }

    shieldSprite.setTexture(shieldTexture, true);
    auto sBounds = shieldSprite.getLocalBounds();
    shieldSprite.setOrigin({sBounds.size.x / 2.f, sBounds.size.y / 2.f});
    shieldSprite.setColor(sf::Color(0, 255, 255, 80));

    sprite.emplace(texture);
    auto shipBounds = sprite->getLocalBounds();
    sprite->setOrigin({shipBounds.size.x / 2.f, shipBounds.size.y / 2.f});

    auto shieldBounds = shieldSprite.getLocalBounds();
    float shipRadius = std::max(shipBounds.size.x, shipBounds.size.y) / 2.f;
    float shieldRadius = std::max(shieldBounds.size.x, shieldBounds.size.y) / 2.f;

    if (shieldRadius < 1.f)
        shieldRadius = 1.f;
    float scaleFactor = (shipRadius + 15.f) / shieldRadius;

    shieldSprite.setScale({scaleFactor, scaleFactor});
    shieldSprite.setPosition({0.f, -5.f});

    setPosition({worldBounds.x / 2.f, worldBounds.y - 100.f});

    float laserFreq = (data.primaryWeapon.cooldown > 0.f) ? (1.f / data.primaryWeapon.cooldown) : 1.f;
    laserLauncher = new LaserLauncher(data.primaryWeapon.name, laserFreq, data.primaryWeapon.speed, data.primaryWeapon.damage);

    float missileFreq = (data.secondaryWeapon.cooldown > 0.f) ? (1.f / data.secondaryWeapon.cooldown) : 0.5f;
    missileLauncher = new MissileLauncher(data.secondaryWeapon.name, missileFreq, data.secondaryWeapon.speed, data.secondaryWeapon.damage);
}

SpaceShip::~SpaceShip()
{
    delete laserLauncher;
    delete missileLauncher;
}

void SpaceShip::setWorldBounds(sf::Vector2f bounds)
{
    worldBounds = bounds;
    setPosition({worldBounds.x / 2.f, worldBounds.y - 100.f});
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
        movement.x -= data.speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += data.speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= data.speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += data.speed * deltaTime;

    move(movement);

    sf::Vector2f pos = getPosition();

    float marginX = 30.f;
    float marginY = 40.f;

    float minX = marginX;
    float maxX = worldBounds.x - marginX;
    float minY = marginY;
    float maxY = worldBounds.y - marginY;

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
        }
    }

    if (rapidFireTimer > 0.f)
    {
        rapidFireTimer -= deltaTime;
        if (rapidFireTimer <= 0.f)
        {
            if (laserLauncher)
            {
                float baseFreq = (data.primaryWeapon.cooldown > 0.f) ? (1.f / data.primaryWeapon.cooldown) : 1.f;
                laserLauncher->setFireRate(baseFreq);
            }
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
}

void SpaceShip::heal(float amount)
{
    health += amount;
    if (health > data.maxHealth)
        health = data.maxHealth;
}

void SpaceShip::enableDoubleShot()
{
    doubleShotActive = true;
}

void SpaceShip::enableRapidFire()
{
    if (laserLauncher)
    {
        float baseFreq = (data.primaryWeapon.cooldown > 0.f) ? (1.f / data.primaryWeapon.cooldown) : 1.f;
        laserLauncher->setFireRate(baseFreq * 3.f);
        rapidFireTimer = 5.0f;
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
            health -= damageRemaining;
        }
        else
        {
            shield -= damageToShield;
            return;
        }
    }
    else
    {
        health -= amount;
    }

    if (health <= 0.f)
    {
        if (onLifeLost)
        {
            onLifeLost(getPosition());
        }

        lives--;
        if (lives > 0)
        {
            respawn();
        }
        else
        {
            Entity::destroy();
        }
    }
}

void SpaceShip::respawn()
{
    health = data.maxHealth;
    shield = maxShield * 0.3f;

    setPosition({worldBounds.x / 2.f, worldBounds.y - 100.f});

    if (sprite)
    {
        sprite->setPosition({0.f, 0.f});
    }

    setInvulnerable(3.0f);
    doubleShotActive = false;
    rapidFireTimer = 0.f;
    if (laserLauncher)
    {
        float baseFreq = (data.primaryWeapon.cooldown > 0.f) ? (1.f / data.primaryWeapon.cooldown) : 1.f;
        laserLauncher->setFireRate(baseFreq);
    }
}

void SpaceShip::setOnLifeLostCallback(OnLifeLostCallback callback)
{
    onLifeLost = callback;
}