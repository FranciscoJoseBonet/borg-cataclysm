#include "MenuScene.h"
#include <iostream>
#include <cmath>

MenuScene::MenuScene(sf::RenderWindow &w)
    : window(w),
      stars(w.getSize(), 400),
      titleText(font)
{
    if (!font.openFromFile("../assets/fonts/Star_Trek_Enterprise_Future.ttf"))
    {
        if (!font.openFromFile("../assets/fonts/pixel_font.ttf"))
            std::cerr << "ERROR CRITICO: No hay fuentes disponibles.\n";
    }

    titleText.setString("BORG CATACLYSM");
    titleText.setCharacterSize(100);
    titleText.setFillColor(sf::Color(255, 215, 0));
    titleText.setOutlineColor(sf::Color(100, 0, 0));
    titleText.setOutlineThickness(4.f);
    centerText(titleText, -200.f);

    initMenuOptions();
}

void MenuScene::initMenuOptions()
{
    std::vector<std::string> labels = {"INICIAR MISION", "REGISTROS DE VUELO", "CONFIGURACION DE NAVE"};

    float startY = 50.f;
    float spacing = 80.f;

    for (size_t i = 0; i < labels.size(); ++i)
    {
        sf::Text text(font);
        text.setString(labels[i]);
        text.setCharacterSize(40);
        text.setFillColor(sf::Color(200, 200, 200));

        centerText(text, startY + (i * spacing));

        menuOptions.push_back(text);
    }
}

void MenuScene::centerText(sf::Text &text, float yOffset)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition({window.getSize().x / 2.f, (window.getSize().y / 2.f) + yOffset});
}

void MenuScene::handleEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                if (selectedItemIndex == 0)
                    nextScene = SceneType::Game;
                else if (selectedItemIndex == 1)
                    nextScene = SceneType::Score;
                else if (selectedItemIndex == 2)
                    nextScene = SceneType::Options;
            }
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
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos);

    selectedItemIndex = -1;

    for (size_t i = 0; i < menuOptions.size(); ++i)
    {
        if (menuOptions[i].getGlobalBounds().contains(mouseWorld))
        {
            selectedItemIndex = static_cast<int>(i);

            menuOptions[i].setFillColor(sf::Color(255, 215, 0));
            menuOptions[i].setScale({1.1f, 1.1f});
        }
        else
        {
            menuOptions[i].setFillColor(sf::Color(200, 200, 200));
            menuOptions[i].setScale({1.0f, 1.0f});
        }
    }
}

void MenuScene::render()
{
    window.clear(sf::Color::Black);
    stars.draw(window);
    window.draw(titleText);

    for (const auto &text : menuOptions)
    {
        window.draw(text);
    }

    window.display();
}