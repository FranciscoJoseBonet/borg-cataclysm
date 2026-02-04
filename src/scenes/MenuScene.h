#pragma once
#include "Scene.h"
#include "../environment/StarField.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class MenuScene : public Scene
{
private:
    sf::RenderWindow &window;

    sf::View view;
    sf::Vector2f baseResolution;

    StarField stars;

    sf::Font font;
    sf::Text titleText;
    std::vector<sf::Text> menuOptions;

    int selectedItemIndex = -1;
    SceneType nextScene = SceneType::None;
    float totalTime = 0.f;

    void initMenuOptions();
    void centerText(sf::Text &text, float yOffset);
    void updateView();

public:
    MenuScene(sf::RenderWindow &window);

    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override { return nextScene; }
};