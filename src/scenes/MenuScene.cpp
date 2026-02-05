#include "MenuScene.h"
#include "../ui/UITheme.h"
#include <iostream>
#include <cmath>

MenuScene::MenuScene(sf::RenderWindow &w)
    : window(w),
      stars(w.getSize(), 400),
      titleText(UITheme::getInstance().getFont())
{
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);

    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    titleText.setString("BORG CATACLYSM");
    titleText.setCharacterSize(100);

    UITheme::applyTitleStyle(titleText);

    centerText(titleText, -200.f);

    initMenuOptions();
}

void MenuScene::initMenuOptions()
{
    std::vector<std::string> labels = {
        "INICIAR MISION",
        "REGISTROS DE VUELO",
        "SALIR AL ESCRITORIO"};

    float startY = 50.f;
    float spacing = 80.f;

    for (size_t i = 0; i < labels.size(); ++i)
    {
        sf::Text text(UITheme::getInstance().getFont());
        text.setString(labels[i]);
        text.setCharacterSize(40);

        UITheme::applyMenuOptionStyle(text, false);

        centerText(text, startY + (i * spacing));

        menuOptions.push_back(text);
    }
}

void MenuScene::centerText(sf::Text &text, float yOffset)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    text.setPosition({baseResolution.x / 2.f,
                      (baseResolution.y / 2.f) + yOffset});
}

void MenuScene::updateView()
{
    float windowRatio = (float)window.getSize().x / (float)window.getSize().y;
    float viewRatio = baseResolution.x / baseResolution.y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    if (horizontalSpacing)
    {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    }
    else
    {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
}

void MenuScene::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Resized>())
    {
        updateView();
    }

    if (const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (selectedItemIndex == 0)
                nextScene = SceneType::Selection;
            else if (selectedItemIndex == 1)
                nextScene = SceneType::Score;
            else if (selectedItemIndex == 2)
                window.close();
        }
    }
}

void MenuScene::update()
{
    float dt = 1.f / 60.f;
    totalTime += dt;
    stars.update(dt);

    float yOffset = std::sin(totalTime * 1.5f) * 10.f;
    centerText(titleText, -200.f + yOffset);

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos, view);

    selectedItemIndex = -1;

    for (size_t i = 0; i < menuOptions.size(); ++i)
    {
        if (menuOptions[i].getGlobalBounds().contains(mouseWorld))
        {
            selectedItemIndex = static_cast<int>(i);
            UITheme::applyMenuOptionStyle(menuOptions[i], true);
        }
        else
        {
            UITheme::applyMenuOptionStyle(menuOptions[i], false);
        }
    }
}

void MenuScene::render()
{
    window.clear(sf::Color::Black);

    window.setView(view);

    stars.draw(window);
    window.draw(titleText);

    for (const auto &text : menuOptions)
    {
        window.draw(text);
    }

    window.display();
}