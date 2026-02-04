#include "ScoreScene.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstdint>

ScoreScene::ScoreScene(sf::RenderWindow &w)
    : window(w),
      background(backgroundTexture),
      titleText(font),
      playAgainButton(font),
      menuButton(font),
      totalTime(0.f)
{
    if (!font.openFromFile("../assets/fonts/Star_Trek_Enterprise_Future.ttf"))
    {
        std::cerr << "ERROR: No se pudo cargar la fuente.\n";
    }

    if (!backgroundTexture.loadFromFile("../assets/img/GAMEOVER_BG.jpg"))
    {
        std::cerr << "Warning: No background texture found.\n";
    }
    else
    {
        background.setTexture(backgroundTexture, true);
        sf::Vector2u texSize = backgroundTexture.getSize();
        sf::Vector2u winSize = window.getSize();
        float scaleX = static_cast<float>(winSize.x) / texSize.x;
        float scaleY = static_cast<float>(winSize.y) / texSize.y;
        float maxScale = std::max(scaleX, scaleY);
        background.setScale({maxScale, maxScale});
    }

    sf::Vector2u winSize = window.getSize();

    panel.setSize({700.f, 650.f});
    panel.setFillColor(sf::Color(0, 0, 0, 180));
    panel.setOutlineColor(sf::Color::White);
    panel.setOutlineThickness(2.f);

    sf::Vector2f pSize = panel.getSize();
    panel.setOrigin({pSize.x / 2.f, pSize.y / 2.f});
    panel.setPosition({winSize.x / 2.f, winSize.y / 2.f});

    titleText.setString("TABLA DE POSICIONES");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::White);
    titleText.setOutlineThickness(1.f);
    centerText(titleText, -280.f);

    setupButton(playAgainButton, "VOLVER A JUGAR", -150.f, 260.f);
    setupButton(menuButton, "MENU PRINCIPAL", 150.f, 260.f);

    loadAndSortScores();
}

void ScoreScene::setupButton(sf::Text &text, std::string label, float xOffset, float yOffset)
{
    text.setString(label);
    text.setCharacterSize(25);
    text.setFillColor(sf::Color(200, 200, 200));

    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    text.setPosition({(window.getSize().x / 2.f) + xOffset,
                      (window.getSize().y / 2.f) + yOffset});
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

        sf::Text text(font);
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
        sf::Text text(font);
        text.setString("NO HAY DATOS AUN");
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        centerText(text, 0.f);
        scoreTexts.push_back(text);
    }
}

void ScoreScene::centerText(sf::Text &text, float yOffset)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition({window.getSize().x / 2.f, (window.getSize().y / 2.f) + yOffset});
}

void ScoreScene::handleEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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
}

void ScoreScene::update()
{
    float dt = 1.f / 60.f;
    totalTime += dt;

    float yOffset = std::sin(totalTime * 2.f) * 5.f;
    centerText(titleText, -280.f + yOffset);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    auto updateButtonHover = [&](sf::Text &btn)
    {
        if (btn.getGlobalBounds().contains(mousePos))
        {
            btn.setFillColor(sf::Color(255, 215, 0));
            btn.setScale({1.1f, 1.1f});
        }
        else
        {
            btn.setFillColor(sf::Color(200, 200, 200));
            btn.setScale({1.0f, 1.0f});
        }
    };

    updateButtonHover(playAgainButton);
    updateButtonHover(menuButton);
}

void ScoreScene::render()
{
    window.clear(sf::Color::Black);

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