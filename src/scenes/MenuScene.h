#pragma once
#include "Scene.h"
#include "../environment/StarField.h"
#include <vector>
#include <string>

class MenuScene : public Scene
{
private:
    sf::RenderWindow &window;

    sf::Font font;

    StarField stars;
    sf::Text titleText;
    std::vector<sf::Text> menuOptions;

    int selectedItemIndex = -1;
    SceneType nextScene = SceneType::None;
    float totalTime = 0.f;

    void initMenuOptions();
    void centerText(sf::Text &text, float yOffset);

public:
    MenuScene(sf::RenderWindow &window);

    void handleEvents() override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override { return nextScene; }
};