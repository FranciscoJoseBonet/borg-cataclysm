#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <optional>
#include "../managers/ResourceManager.h"

class HUD
{
private:
    // Txts
    sf::Text scoreText;
    sf::Text levelText;
    float levelTextTimer; // Tiempo para desvanecer el texto de Level X, para animacion de vanish

    // Barras de estado
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBack; // Fondo de la barra
    sf::RectangleShape shieldBar;
    sf::RectangleShape shieldBarBack;

    // Iconos
    // Mapita para los powerups por si agregamos mas
    std::map<std::string, sf::Sprite> powerUpIcons;

    std::optional<sf::Sprite> healthIcon;
    std::optional<sf::Sprite> shieldIcon;
    std::optional<sf::Sprite> shipLifeIcon; // Icono de las vidas restantes

    // UI
    sf::Vector2f baseResolution;
    float widthBar = 200.f;
    float heightBar = 20.f;
    int currentLives = 0;

public:
    HUD();

    // Inicializa texturas y posiciones base
    void init(const sf::Vector2f &resolution, ResourceManager &resources);

    void update(float dt);

    // Muestra el texto gigante de LEVEL X
    void showLevelMessage(int level);

    // Actualiza barras y puntaje
    void updateStats(int score, int lives, float hp, float maxHp, float shield, float maxShield);

    // Muestra/Oculta iconos de powerups activos
    void updatePowerUps(bool doubleShot, bool rapidFire, bool invulnerable);

    void draw(sf::RenderTarget &target);
};