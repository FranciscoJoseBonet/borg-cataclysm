#pragma once
#include "Weapon.h"

class MissileLauncher : public Weapon
{
public:
    // Usamos los mismos constructores que la clase Weapon usando herencia de constructores
    using Weapon::Weapon;

    // Sobrescribimos el disparo para lanzar misiles y ajustar la posicion de salida
    void Shoot(const sf::Vector2f &startPos) override;
};