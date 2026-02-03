#include "LaserLauncher.h"

void LaserLauncher::Shoot(const sf::Vector2f &startPos)
{
    if (currentCooldown > 0.f)
        return;
    currentCooldown = cooldownTime;

    sf::Vector2f spawnPos = startPos;
    spawnPos.y -= 100.f;

    if (onFire)
    {
        onFire(ProjectileType::LASER, spawnPos, damage, projectileSpeed);
    }
}