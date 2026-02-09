#pragma once
#include "../Entity.h"
#include <SFML/Graphics.hpp>

// Tipos de mejoras disponibles en el juego
enum class PowerUpType
{
    SHIELD,
    RAPID_FIRE,
    DOUBLE_SHOT,
    INVINCIBILITY,
    HEAL,
    EXTRA_LIFE
};

class PowerUp : public Entity
{
private:
    PowerUpType powerUpType;
    float speed;
    sf::Sprite sprite;

    // Metodo interno de dibujado (heredado de Entity/Drawable)
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    PowerUp(sf::Vector2f position, PowerUpType type, const sf::Texture &texture);

    // Override de metodos de Entity
    void update(float deltaTime) override;
    sf::FloatRect getBounds() const override;

    // Getter especifico para saber que efecto aplicar al jugador
    PowerUpType getPowerUpType() const { return powerUpType; }
};