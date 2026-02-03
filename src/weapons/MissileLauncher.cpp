#include "MissileLauncher.h"

void MissileLauncher::Shoot(const sf::Vector2f &startPos)
{

    if (currentCooldown > 0.f)
        return;
    currentCooldown = cooldownTime;

    sf::Vector2f spawnPos = startPos;
    spawnPos.y -= 115.f;

    if (onFire)
    {
        onFire(ProjectileType::MISSILE, spawnPos, damage, projectileSpeed);
    }
}