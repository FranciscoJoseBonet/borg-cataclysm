#include "MissileLauncher.h"

Projectile *MissileLauncher::Shoot(const sf::Vector2f &startPos)
{
    if (currentCooldown > 0.f)
        return nullptr;

    currentCooldown = cooldownTime;
    sf::Vector2f direction(0.f, -1.f);

    MissileProjectile *m = new MissileProjectile(direction, projectileSpeed, 800.f, damage);

    sf::Vector2f spawnPos = startPos;
    spawnPos.y -= 115.f;

    m->setPosition(spawnPos);

    return m;
}