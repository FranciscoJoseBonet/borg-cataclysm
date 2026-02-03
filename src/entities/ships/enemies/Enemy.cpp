#include "Enemy.h"

Enemy::Enemy(float health, const std::string &type, float fireRate)
    : Entity(health, type, Faction::Alien),
      fireTimer(0.f)
{
    fireInterval = (fireRate > 0) ? (1.f / fireRate) : 0.f;
}

Enemy::~Enemy()
{
    delete weapon;
}

void Enemy::setFireCallback(OnFireCallback callback)
{
    onFire = callback;
    if (weapon)
        weapon->setCallback(callback);
}

void Enemy::update(float deltaTime)
{

    movePattern(deltaTime);

    fireTimer += deltaTime;

    if (fireTimer >= fireInterval && getPosition().y > 0)
    {
        if (weapon)
        {
            weapon->Shoot(getPosition() + sf::Vector2f(0.f, 120.f));
        }
        fireTimer = 0.f;
    }

    if (weapon)
        weapon->update(deltaTime);

    if (getPosition().y > 1100.f)
    {
        setPosition({getPosition().x, -100.f});
    }
}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}

void Enemy::setFireRate(float rate)
{
    fireInterval = (rate > 0) ? (1.f / rate) : 0.f;
    fireTimer = 0.f;
}