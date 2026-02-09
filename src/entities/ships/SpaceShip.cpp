#include "SpaceShip.h"
#include <iostream>
#include <algorithm>
#include "../../weapons/LaserLauncher.h"
#include "../../weapons/MissileLauncher.h"

SpaceShip::SpaceShip(const ShipData &shipData, const sf::Texture &texture)
    : Entity(shipData.maxHealth, shipData.name, Faction::Player),
      data(shipData),
      worldBounds({1280.f, 720.f}),
      sprite(texture),
      shieldSprite(shieldTexture)
{
    // 1. Configuracion de Stats
    maxShield = data.maxHealth; // Escudo maximo igual a la vida base
    shield = maxShield;

    // 2. Configuracion Visual de la Nave
    auto shipBounds = sprite.getLocalBounds();
    sprite.setOrigin({shipBounds.size.x / 2.f, shipBounds.size.y / 2.f});

    // Posicion inicial en el medio abajo
    setPosition({worldBounds.x / 2.f, worldBounds.y - 100.f});

    // 3. Carga y Configuracion del Escudo
    if (!shieldTexture.loadFromFile(data.shieldTexturePath))
    {
        std::cerr << "Error: No se encontro imagen escudo: " << data.shieldTexturePath << "\n";

        // Fallback visual simple por si no me carga el asset
        sf::Image temp;
        temp.resize({100, 100}, sf::Color::Cyan);
        (void)shieldTexture.loadFromImage(temp);
    }

    shieldSprite.setTexture(shieldTexture, true);
    auto sBounds = shieldSprite.getLocalBounds();
    shieldSprite.setOrigin({sBounds.size.x / 2.f, sBounds.size.y / 2.f});
    shieldSprite.setColor(sf::Color(0, 255, 255, 80)); // Semi-transparente

    // Calculo para que el escudo cubra a la nave independientemente del tamaño
    float shipRadius = std::max(shipBounds.size.x, shipBounds.size.y) / 2.f;
    float shieldRadius = std::max(sBounds.size.x, sBounds.size.y) / 2.f;
    if (shieldRadius < 1.f)
        shieldRadius = 1.f;

    float scaleFactor = (shipRadius + 15.f) / shieldRadius;
    shieldSprite.setScale({scaleFactor, scaleFactor});
    shieldSprite.setPosition({0.f, -5.f}); // Ajuste fino visual del escudo para que no quede muy lejos de la nave, ya que confunde al jugador on la hb

    // 4. Inicializacion de Armas en memoria dinamica
    // Usamos el cooldown inverso para calcular la frecuencia
    float laserFreq = (data.primaryWeapon.cooldown > 0.f) ? (1.f / data.primaryWeapon.cooldown) : 1.f;
    laserLauncher = new LaserLauncher(data.primaryWeapon.name, laserFreq, data.primaryWeapon.speed, data.primaryWeapon.damage);

    float missileFreq = (data.secondaryWeapon.cooldown > 0.f) ? (1.f / data.secondaryWeapon.cooldown) : 0.5f;
    missileLauncher = new MissileLauncher(data.secondaryWeapon.name, missileFreq, data.secondaryWeapon.speed, data.secondaryWeapon.damage);
}

SpaceShip::~SpaceShip()
{
    // Destructor
    delete laserLauncher;
    delete missileLauncher;
}

// ciclo de juego

void SpaceShip::update(float deltaTime)
{
    // A. Movimiento de la nave con WASD
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

    // B. Limites en la pantalla
    sf::Vector2f pos = getPosition();
    float marginX = 30.f;
    float marginY = 40.f;

    // Limitamos manualmente por donde nos podemos mover
    if (pos.x < marginX)
        pos.x = marginX;
    else if (pos.x > worldBounds.x - marginX)
        pos.x = worldBounds.x - marginX;

    if (pos.y < marginY)
        pos.y = marginY;
    else if (pos.y > worldBounds.y - marginY)
        pos.y = worldBounds.y - marginY;

    setPosition(pos);

    // C. Actualizamos los estados

    // El blink de la invulnerabilidad
    if (isInvulnerable)
    {
        invulnerabilityTimer -= deltaTime;
        blinkTimer += deltaTime;

        if (blinkTimer >= 0.1f) // blink rapido
        {
            blinkTimer = 0.f;
            isBlinkVisible = !isBlinkVisible;

            sf::Color c = sprite.getColor();
            c.a = isBlinkVisible ? 128 : 50; // Alternar transparencia
            sprite.setColor(c);
        }

        if (invulnerabilityTimer <= 0.f)
        {
            isInvulnerable = false;
            sprite.setColor(sf::Color::White); // Restaurar color normal
        }
    }

    // Logica de Disparo Rapido
    if (rapidFireTimer > 0.f)
    {
        rapidFireTimer -= deltaTime;
        if (rapidFireTimer <= 0.f)
        {
            // Volver a la cadencia normal
            if (laserLauncher)
            {
                float baseFreq = (data.primaryWeapon.cooldown > 0.f) ? (1.f / data.primaryWeapon.cooldown) : 1.f;
                laserLauncher->setFireRate(baseFreq);
            }
        }
    }

    // D. Actualizacion de las armas y logica de disparo
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
    target.draw(sprite, states);

    // El escudo se dibuja encima de la nave si tiene carga
    if (shield > 0)
    {
        target.draw(shieldSprite, states);
    }
}

sf::FloatRect SpaceShip::getBounds() const
{
    return getTransform().transformRect(sprite.getGlobalBounds());
}

// Logica del daño y efectos de los PU

void SpaceShip::takeDamage(float amount)
{
    if (isInvulnerable)
        return;

    // 1. Absorcion por Escudo
    if (shield > 0)
    {
        float damageToShield = amount;

        if (damageToShield > shield)
        {
            // El escudo se rompe y el daño sobrante va a la vida
            float damageRemaining = damageToShield - shield;
            shield = 0;
            health -= damageRemaining;
        }
        else
        {
            shield -= damageToShield;
            return; // El escudo absorbió todo
        }
    }
    else
    {
        // 2. Daño directo al casco
        health -= amount;
    }

    // 3. Muerte y Respawn
    if (health <= 0.f)
    {
        if (onLifeLost)
            onLifeLost(getPosition());

        lives--;
        if (lives > 0)
        {
            respawn();
        }
        else
        {
            Entity::destroy(); // Game Over real
        }
    }
}

void SpaceShip::respawn()
{
    health = data.maxHealth;
    shield = maxShield * 0.3f; // Respawn con poco escudo

    setPosition({worldBounds.x / 2.f, worldBounds.y - 100.f});

    // Resetear buffs
    setInvulnerable(3.0f);
    doubleShotActive = false;
    rapidFireTimer = 0.f;

    // Resetear arma primaria
    if (laserLauncher)
    {
        float baseFreq = (data.primaryWeapon.cooldown > 0.f) ? (1.f / data.primaryWeapon.cooldown) : 1.f;
        laserLauncher->setFireRate(baseFreq);
    }
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

// Algunos helpers internos

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
        // Triplicamos la cadencia
        laserLauncher->setFireRate(baseFreq * 2.f);
        rapidFireTimer = 5.0f; // Dura 5 segundos
    }
}

void SpaceShip::setInvulnerable(float duration)
{
    isInvulnerable = true;
    invulnerabilityTimer = duration;

    // Reiniciar parpadeo
    blinkTimer = 0.f;
    isBlinkVisible = true;
    sprite.setColor(sf::Color(255, 255, 255, 128));
}

// setters

void SpaceShip::setWorldBounds(sf::Vector2f bounds)
{
    worldBounds = bounds;
    // Si cambia la resolucion, recolocamos la nave
    setPosition({worldBounds.x / 2.f, worldBounds.y - 100.f});
}

void SpaceShip::setWeaponsCallback(OnFireCallback callback)
{
    if (laserLauncher)
        laserLauncher->setCallback(callback);
    if (missileLauncher)
        missileLauncher->setCallback(callback);
}

void SpaceShip::setOnLifeLostCallback(OnLifeLostCallback callback)
{
    onLifeLost = callback;
}