#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"
#include "../../weapons/Weapon.h"
#include "../items/PowerUp.h"
#include <optional>
#include <memory>

class SpaceShip : public Entity
{
private:
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;
    int lives = 3;
    float speed = 300.f;
    float invulnerabilityTimer = 0.f;
    bool isInvulnerable = false;
    float rapidFireTimer = 0.f;
    bool doubleShotActive = false;
    float blinkTimer = 0.f;
    bool isBlinkVisible = true;
    float shield;
    float maxShield;

    sf::Vector2f worldBounds;

    sf::Texture shieldTexture;
    sf::Sprite shieldSprite;

    Weapon *laserLauncher = nullptr;
    Weapon *missileLauncher = nullptr;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void enableDoubleShot();
    void enableRapidFire();
    void heal(float amount);
    void setInvulnerable(float duration);
    void addShield(float amount);

    void respawn();

public:
    SpaceShip();
    ~SpaceShip();

    void setWorldBounds(sf::Vector2f bounds);
    void setWeaponsCallback(OnFireCallback callback);
    sf::FloatRect getBounds() const override;
    void destroy() override;
    void update(float deltaTime) override;
    void takeDamage(float amount) override;
    void applyPowerUp(PowerUpType type);
    bool isDoubleShotActive() const { return doubleShotActive; }
    bool isInvulnerableState() const { return isInvulnerable; }
    int getLives() const { return lives; }
};