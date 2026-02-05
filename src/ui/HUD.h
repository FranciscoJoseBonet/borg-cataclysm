#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <optional>
#include "../managers/ResourceManager.h"

class HUD
{
private:
    sf::Font font;
    sf::Text scoreText;

    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBack;
    sf::RectangleShape shieldBar;
    sf::RectangleShape shieldBarBack;

    std::map<std::string, sf::Sprite> powerUpIcons;

    std::optional<sf::Sprite> healthIcon;
    std::optional<sf::Sprite> shieldIcon;
    std::optional<sf::Sprite> shipLifeIcon;

    sf::Vector2f baseResolution;

    float widthBar = 200.f;
    float heightBar = 20.f;
    int currentLives = 0;

public:
    HUD();

    void init(const sf::Vector2f &resolution, ResourceManager &resources);

    void updateStats(int score, int lives, float hp, float maxHp, float shield, float maxShield);
    void updatePowerUps(bool doubleShot, bool rapidFire, bool invulnerable);

    void draw(sf::RenderWindow &window);
};