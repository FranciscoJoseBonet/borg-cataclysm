#include "MissileLauncher.h"

void MissileLauncher::Shoot(const sf::Vector2f &startPos)
{
    // 1. Chequeo de Cooldown, la tenemos que definir de nuevo porque esta todo overrideado
    if (currentCooldown > 0.f)
        return;

    currentCooldown = cooldownTime;

    // 2. Calculo de Posicion de Salida
    // el misil sale un poco mas adelante que el laser
    sf::Vector2f spawnPos = startPos;
    spawnPos.y -= 115.f;

    // 3. Disparo especifico de misil, no el laser default
    if (onFire)
    {
        onFire(ProjectileType::MISSILE, spawnPos, damage, projectileSpeed);
    }
}