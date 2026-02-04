#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct ScoreEntry
{
    std::string name;
    int points;
};

class ScoreScene : public Scene
{
private:
    sf::RenderWindow &window;

    sf::Font font;
    sf::Texture backgroundTexture;

    sf::Sprite background;
    sf::RectangleShape panel;

    sf::Text titleText;
    sf::Text infoText;
    std::vector<sf::Text> scoreTexts;

    float totalTime;
    bool requestRestart = false;

    void loadAndSortScores();
    void centerText(sf::Text &text, float yOffset);

public:
    ScoreScene(sf::RenderWindow &window);

    SceneType getNextScene() const override
    {
        return requestRestart ? SceneType::Game : SceneType::None;
    }

    void handleEvents() override;
    void update() override;
    void render() override;
};