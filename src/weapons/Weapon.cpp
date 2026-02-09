#include "Weapon.h"

Weapon::Weapon(const std::string &name, float fireRate, float speed, int dmg)
    : name(name),
      fireRate(fireRate),
      projectileSpeed(speed),
      damage(dmg),
      currentCooldown(0.f)
{
    // Usamos el setter para calcular el cooldownTime correctamente desde el inicio
    setFireRate(fireRate);
}

void Weapon::setFireRate(float rate)
{
    fireRate = rate;

    // para no dividir por cero
    if (rate > 0.f)
    {
        cooldownTime = 1.f / rate;
    }
    else
    {
        cooldownTime = 0.f;
    }
}

void Weapon::update(float deltaTime)
{
    // Solo restamos tiempo si el arma esta en coolDown
    if (currentCooldown > 0.f)
    {
        currentCooldown -= deltaTime;
    }
}

void Weapon::Shoot(const sf::Vector2f &startPos)
{
    // 1. Chequeo de Cooldown
    if (currentCooldown > 0.f)
        return; // Todavia no se puede usae

    // 2. Reiniciar el Cooldown
    currentCooldown = cooldownTime;

    // 3. Ejecutar el Callback, notifica al lvl manager
    if (onFire)
    {
        // Por defecto, la clase base dispara un laser
        // Las clases hijas deberian cambiar esto si disparan otra cosa
        onFire(ProjectileType::LASER, startPos, damage, projectileSpeed);
    }
}