#include "Enemy.h"

Enemy::Enemy(const sf::Texture &texture, float health, const std::string &type, float fireRate)
    : Entity(health, type, Faction::Alien),
      sprite(texture),
      fireTimer(0.f),
      fireInterval(fireRate),
      speed(100.f),
      projectileSpeed(300.f),
      damage(10)
{
    // Configuracion basica del Sprite
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
}

// Config

void Enemy::setFireCallback(OnFireCallback callback)
{
    onFire = callback;
}

void Enemy::setFireRate(float rate)
{
    fireInterval = rate;
}

void Enemy::setProjectileSpeed(float s)
{
    projectileSpeed = s;
}

// Ciclo de vida

void Enemy::update(float deltaTime)
{
    // 1. Logica de Disparo
    // Empiezan a disparar cuando estan cerca de entrar en la pantalla
    if (getPosition().y > -150.f)
    {
        fireTimer += deltaTime;
        if (fireTimer >= fireInterval)
        {
            fireTimer = 0.f; // Reset del timer
            if (onFire)
            { // Dispara un laser común desde su posicion
                onFire(ProjectileType::LASER, getPosition(), damage, projectileSpeed);
            }
        }
    }

    // 2. Movimiento (Delegado a la clase hija el scout, explorer o boss)
    movePattern(deltaTime);

    // 3. Logica de respawn para los que se van del mapa
    // para que vuelva a bajar, asi la wave no termina hasta que el jugador lo mate
    if (getPosition().y > 1200.f)
    {
        setPosition({getPosition().x, -200.f});
    }
}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Aplicamos transformacion y dibujamos
    states.transform *= getTransform();
    target.draw(sprite, states);
}

sf::FloatRect Enemy::getBounds() const
{
    // Devolvemos la caja de colision ajustada a la posicion real
    return getTransform().transformRect(sprite.getGlobalBounds());
}