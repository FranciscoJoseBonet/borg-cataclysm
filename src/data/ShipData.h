#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

struct WeaponData
{
    std::string name;
    float cooldown;          // Tiempo en segundos entre disparos
    float speed;             // Velocidad del proyectil
    int damage;              // Daño por impacto
    std::string texturePath; // Sprite del proyectil
};

struct ShipData
{
    // Identificación
    std::string name;
    std::string texturePath;

    // Estadísticas de la Nave
    float maxHealth;
    float speed;

    // Configuración de Armas
    WeaponData primaryWeapon;
    WeaponData secondaryWeapon;

    // Recursos de Defensa
    std::string shieldTexturePath;

    // Configuración de Efectos
    std::string explosionTexturePath;
    sf::Vector2i explosionFrameSize;
    int explosionNumFrames;

    // Info de Selección
    std::string description;
    std::string planetBackgroundPath;
};