#pragma once
#include "../Entity.h"
#include "../../weapons/Weapon.h"
#include "../items/PowerUp.h"
#include "../../data/ShipData.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>

// Callback para avisar a la escena que perdimos una vida (para explosiones o UI)
using OnLifeLostCallback = std::function<void(sf::Vector2f)>;

class SpaceShip : public Entity
{
private:
    // Datos generales de la nave del jugador
    ShipData data;
    sf::Vector2f worldBounds; // Limites de la pantalla para no salirnos
    int lives = 3;

    // Estado del jugador
    bool isInvulnerable = false;
    float invulnerabilityTimer = 0.f;

    bool doubleShotActive = false;

    float rapidFireTimer = 0.f; // Si es mayor a 0, tenemos disparo rapido

    // Escudos
    float shield;
    float maxShield;

    // Assets de la nave
    sf::Sprite sprite; // Sprite principal de la nave

    sf::Texture shieldTexture; // Textura propia del escudo
    sf::Sprite shieldSprite;

    // Variables para el efecto de parpadeo al ser invulnerable
    float blinkTimer = 0.f;
    bool isBlinkVisible = true;

    // Armas
    Weapon *laserLauncher = nullptr;
    Weapon *missileLauncher = nullptr;

    // Callbacks
    OnLifeLostCallback onLifeLost = nullptr;

    // Netidos privados
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    // Logica interna de powerups
    void enableDoubleShot();
    void enableRapidFire();
    void heal(float amount);
    void setInvulnerable(float duration);
    void addShield(float amount);

    // Logica de reinicio despues de morir
    void respawn();

public:
    SpaceShip(const ShipData &shipData, const sf::Texture &texture);
    virtual ~SpaceShip(); // Importante para borrar las armas

    // Ciclo de vida
    void update(float deltaTime) override;
    sf::FloatRect getBounds() const override;

    // Combate
    void takeDamage(float amount) override;
    void applyPowerUp(PowerUpType type);

    // Setters
    void setWorldBounds(sf::Vector2f bounds);
    void setWeaponsCallback(OnFireCallback callback); // Conecta las armas con el EntityManager
    void setOnLifeLostCallback(OnLifeLostCallback callback);

    // Getters
    bool isDoubleShotActive() const { return doubleShotActive; }
    bool isInvulnerableState() const { return isInvulnerable; }
    bool isRapidFireActive() const { return rapidFireTimer > 0.f; }

    int getLives() const { return lives; }
    float getShield() const { return shield; }
    float getMaxShield() const { return maxShield; }

    const ShipData &getData() const { return data; }
};