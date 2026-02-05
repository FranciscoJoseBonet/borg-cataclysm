#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include "../managers/ResourceManager.h"
#include <vector>
#include <optional>

class SelectionScene : public Scene
{
private:
    sf::RenderWindow &window;
    ResourceManager resources;

    sf::View view;
    sf::Vector2f baseResolution;

    sf::Font font;

    sf::Text titleText;
    sf::Text shipNameText;
    sf::Text descriptionText;

    sf::Text leftArrow;
    sf::Text rightArrow;

    std::optional<sf::Sprite> shipPreviewSprite;
    std::optional<sf::Sprite> backgroundSprite;

    int currentIndex = 0;
    SceneType nextScene = SceneType::None;

    void updateUI();
    void updateView();

    void drawStatBar(sf::RenderTarget &target, const std::string &label, float value, float maxValue, float yPos, sf::Color color);

public:
    SelectionScene(sf::RenderWindow &window);

    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override { return nextScene; }
};