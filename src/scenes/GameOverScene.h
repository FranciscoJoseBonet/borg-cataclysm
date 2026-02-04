#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <cmath>

class GameOverScene : public Scene
{
private:
    sf::RenderWindow &window;
    int score;
    bool saved;

    sf::Font font;
    sf::Texture backgroundTexture;

    sf::Sprite background;
    sf::RectangleShape panel;

    sf::Text titleText;
    sf::Text scoreText;
    sf::Text promptText;
    sf::Text nameInputText;
    sf::Text infoText;

    float totalTime;
    std::string playerName;

    void saveScoreToFile();
    void centerText(sf::Text &text, float yOffset);

public:
    GameOverScene(sf::RenderWindow &window, int finalScore);

    void handleEvents() override;
    void update() override;
    void render() override;
};