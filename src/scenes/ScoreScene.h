#pragma once
#include "Scene.h"
#include "../ui/UITheme.h"
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

    sf::View view;
    sf::Vector2f baseResolution;

    sf::Texture backgroundTexture;

    sf::Sprite background;
    sf::RectangleShape panel;
    sf::Text titleText;

    sf::Text playAgainButton;
    sf::Text menuButton;

    std::vector<sf::Text> scoreTexts;

    float totalTime;
    SceneType nextScene = SceneType::None;

    void loadAndSortScores();
    void centerText(sf::Text &text, float yOffset);
    void setupButton(sf::Text &text, std::string label, float xOffset, float yOffset);
    void updateView();

public:
    ScoreScene(sf::RenderWindow &window);

    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override { return nextScene; }
};