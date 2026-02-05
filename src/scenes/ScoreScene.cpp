#include "ScoreScene.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstdint>

ScoreScene::ScoreScene(sf::RenderWindow &w)
    : window(w),
      background(backgroundTexture),
      titleText(UITheme::getInstance().getFont()),
      playAgainButton(UITheme::getInstance().getFont()),
      menuButton(UITheme::getInstance().getFont()),
      totalTime(0.f)
{
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);
    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    if (!backgroundTexture.loadFromFile("../assets/img/GAMEOVER_BG.jpg"))
    {
        std::cerr << "Warning: No background texture found.\n";
    }
    else
    {
        background.setTexture(backgroundTexture, true);
        sf::Vector2u texSize = backgroundTexture.getSize();

        float scaleX = baseResolution.x / static_cast<float>(texSize.x);
        float scaleY = baseResolution.y / static_cast<float>(texSize.y);
        float maxScale = std::max(scaleX, scaleY);
        background.setScale({maxScale, maxScale});
    }

    panel.setSize({700.f, 650.f});
    panel.setFillColor(sf::Color(0, 0, 0, 180));
    panel.setOutlineColor(sf::Color::White);
    panel.setOutlineThickness(2.f);

    sf::Vector2f pSize = panel.getSize();
    panel.setOrigin({pSize.x / 2.f, pSize.y / 2.f});
    panel.setPosition({baseResolution.x / 2.f, baseResolution.y / 2.f});

    titleText.setString("TABLA DE POSICIONES");
    titleText.setCharacterSize(60);

    UITheme::applyTitleStyle(titleText);

    centerText(titleText, -280.f);

    setupButton(playAgainButton, "VOLVER A JUGAR", -150.f, 260.f);
    setupButton(menuButton, "MENU PRINCIPAL", 150.f, 260.f);

    loadAndSortScores();
}

void ScoreScene::updateView()
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

void ScoreScene::setupButton(sf::Text &text, std::string label, float xOffset, float yOffset)
{
    text.setString(label);
    text.setCharacterSize(25);

    UITheme::applyMenuOptionStyle(text, false);

    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    text.setPosition({(baseResolution.x / 2.f) + xOffset,
                      (baseResolution.y / 2.f) + yOffset});
}

void ScoreScene::loadAndSortScores()
{
    std::vector<ScoreEntry> entries;
    std::ifstream file("scores.txt");

    if (file.is_open())
    {
        std::string name;
        int points;
        while (file >> name >> points)
            entries.push_back({name, points});
        file.close();
    }

    std::sort(entries.begin(), entries.end(), [](const ScoreEntry &a, const ScoreEntry &b)
              { return a.points > b.points; });

    int count = 0;
    float startY = -180.f;
    float spacing = 40.f;

    for (const auto &entry : entries)
    {
        if (count >= 10)
            break;

        std::string line = std::to_string(count + 1) + ". " + entry.name;
        while (line.length() < 20)
            line += " ";
        line += std::to_string(entry.points);

        sf::Text text(UITheme::getInstance().getFont());
        text.setString(line);
        text.setCharacterSize(30);

        if (count == 0)
            text.setFillColor(sf::Color(255, 215, 0));
        else if (count == 1)
            text.setFillColor(sf::Color(192, 192, 192));
        else if (count == 2)
            text.setFillColor(sf::Color(205, 127, 50));
        else
            text.setFillColor(sf::Color::White);

        centerText(text, startY + (count * spacing));
        scoreTexts.push_back(text);
        count++;
    }

    if (scoreTexts.empty())
    {
        sf::Text text(UITheme::getInstance().getFont());
        text.setString("NO HAY DATOS AUN");
        text.setCharacterSize(30);

        UITheme::applyBodyStyle(text);

        centerText(text, 0.f);
        scoreTexts.push_back(text);
    }
}

void ScoreScene::centerText(sf::Text &text, float yOffset)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition({baseResolution.x / 2.f, (baseResolution.y / 2.f) + yOffset});
}

void ScoreScene::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Resized>())
    {
        updateView();
    }

    if (const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

            if (playAgainButton.getGlobalBounds().contains(mousePos))
            {
                nextScene = SceneType::Game;
            }
            else if (menuButton.getGlobalBounds().contains(mousePos))
            {
                nextScene = SceneType::Menu;
            }
        }
    }
}

void ScoreScene::update()
{
    float dt = 1.f / 60.f;
    totalTime += dt;

    float yOffset = std::sin(totalTime * 2.f) * 5.f;
    centerText(titleText, -280.f + yOffset);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

    auto updateButtonHover = [&](sf::Text &btn)
    {
        bool isHover = btn.getGlobalBounds().contains(mousePos);
        UITheme::applyMenuOptionStyle(btn, isHover);
    };

    updateButtonHover(playAgainButton);
    updateButtonHover(menuButton);
}

void ScoreScene::render()
{
    window.clear(sf::Color::Black);
    window.setView(view);

    window.draw(background);
    window.draw(panel);
    window.draw(titleText);

    for (const auto &text : scoreTexts)
    {
        window.draw(text);
    }

    window.draw(playAgainButton);
    window.draw(menuButton);

    window.display();
}