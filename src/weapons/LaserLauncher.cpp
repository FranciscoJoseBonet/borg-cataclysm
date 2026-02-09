#include "LaserLauncher.h"

void LaserLauncher::Shoot(const sf::Vector2f &startPos)
{
    if (currentCooldown > 0.f)
        return;

    currentCooldown = cooldownTime;

    // 2. Calculo de Posicion de Salida
    // el laser sale desde la punta de los cañones (cerquita de la nave, no usamos sprite para las wepons porque es muy chiquito)
    sf::Vector2f spawnPos = startPos;
    spawnPos.y -= 80.f;

    // 3. Disparo de laser
    if (onFire)
    {
        onFire(ProjectileType::LASER, spawnPos, damage, projectileSpeed);
    }
}