#include "Enemy.h"

Enemy::Enemy(const sf::Texture &texture, float health, const std::string &type, float fireRate)
    : Entity(health, type, Faction::Alien),
      fireTimer(0.f),
      fireInterval(fireRate),
      speed(100.f),
      projectileSpeed(300.f),
      damage(10)
{
    sprite.emplace(texture);
    if (sprite)
    {
        auto bounds = sprite->getLocalBounds();
        sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        sprite->setRotation(sf::degrees(180.f));
    }
}

Enemy::~Enemy() {}

void Enemy::setFireCallback(OnFireCallback callback) { onFire = callback; }
void Enemy::setFireRate(float rate) { fireInterval = rate; }
void Enemy::setProjectileSpeed(float s) { projectileSpeed = s; }

void Enemy::update(float deltaTime)
{
    fireTimer += deltaTime;
    if (fireTimer >= fireInterval)
    {
        fireTimer = 0;
        if (onFire)
        {
            onFire(ProjectileType::LASER, getPosition(), damage, projectileSpeed);
        }
    }

    movePattern(deltaTime);

    if (getPosition().y > 1200.f)
    {
        setPosition({getPosition().x, -200.f});
    }
}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (sprite)
    {
        states.transform *= getTransform();
        target.draw(*sprite, states);
    }
}

sf::FloatRect Enemy::getBounds() const
{
    if (sprite)
    {
        return getTransform().transformRect(sprite->getGlobalBounds());
    }
    return sf::FloatRect();
}