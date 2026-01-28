#include "LaserLauncher.h"
#include "../entities/projectiles/LaserProjectile.h"

Projectile *LaserLauncher::Shoot(const sf::Vector2f &startPos)
{
    if (currentCooldown > 0.f)
        return nullptr;

    currentCooldown = cooldownTime;

    sf::Vector2f direction(0.f, -1.f);

    Projectile *p = new LaserProjectile(
        direction,
        projectileSpeed,
        damage);

    sf::Vector2f spawnPos = startPos;
    spawnPos.y -= 100.f;

    p->setPosition(spawnPos);
    return p;
}
