#include "GameOverScene.h"
#include <iostream>
#include <filesystem>
#include <cstdint>

GameOverScene::GameOverScene(sf::RenderWindow &w, int finalScore)
    : window(w),
      score(finalScore),
      saved(false),
      background(backgroundTexture),

      titleText(font),
      scoreText(font),
      promptText(font),
      nameInputText(font),
      infoText(font),
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

    panel.setSize({600.f, 500.f});
    panel.setFillColor(sf::Color(0, 0, 0, 180));
    panel.setOutlineColor(sf::Color::White);
    panel.setOutlineThickness(2.f);

    sf::Vector2f pSize = panel.getSize();
    panel.setOrigin({pSize.x / 2.f, pSize.y / 2.f});
    panel.setPosition({winSize.x / 2.f, winSize.y / 2.f});

    titleText.setString("GAME OVER");
    titleText.setCharacterSize(90);
    titleText.setFillColor(sf::Color(255, 0, 0));

    titleText.setOutlineColor(sf::Color(255, 125, 125));
    titleText.setOutlineThickness(2.f);
    centerText(titleText, -220.f);

    scoreText.setString("PUNTUACION: " + std::to_string(score));
    scoreText.setCharacterSize(45);
    scoreText.setFillColor(sf::Color::White);
    centerText(scoreText, -100.f);

    promptText.setString("INGRESE SU NOMBRE CADETE:");
    promptText.setCharacterSize(35);
    promptText.setFillColor(sf::Color(222, 156, 29));
    centerText(promptText, 0.f);

    nameInputText.setString("_");
    nameInputText.setCharacterSize(60);
    nameInputText.setFillColor(sf::Color(222, 156, 29));
    centerText(nameInputText, 60.f);

    infoText.setString("[ ENTER PARA GUARDAR - EL KOBAYASHI MARU ERA MAS FACIL... ]");
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color(255, 255, 255));
    centerText(infoText, 200.f);

    infoText.setOutlineColor(sf::Color::Black);
    infoText.setOutlineThickness(1.f);
}

void GameOverScene::centerText(sf::Text &text, float yOffset)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition({window.getSize().x / 2.f, (window.getSize().y / 2.f) + yOffset});
}

void GameOverScene::handleEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto *textEvent = event->getIf<sf::Event::TextEntered>())
        {
            uint32_t unicode = textEvent->unicode;

            if (unicode == 8)
            {
                if (!playerName.empty())
                    playerName.pop_back();
            }
            else if (unicode == 13)
            {
                if (!playerName.empty() && !saved)
                {
                    saveScoreToFile();
                    saved = true;
                    window.close();
                }
            }
            else if (unicode >= 32 && unicode < 128 && playerName.size() < 10)
            {
                playerName += static_cast<char>(unicode);
            }
        }
    }
}

void GameOverScene::saveScoreToFile()
{
    std::ofstream file("scores.txt", std::ios::app);
    if (file.is_open())
    {
        file << playerName << " " << score << "\n";
        file.close();
        std::cout << "Score saved.\n";
    }
}

void GameOverScene::update()
{
    float dt = 1.f / 60.f;
    totalTime += dt;

    nameInputText.setString(playerName + "_");
    centerText(nameInputText, 60.f);

    std::uint8_t alpha = static_cast<std::uint8_t>(155 + 100 * std::sin(totalTime * 3.f));

    float yOffset = std::sin(totalTime * 2.f) * 10.f;
    centerText(titleText, -220.f + yOffset);
}

void GameOverScene::render()
{
    window.clear(sf::Color::Black);

    window.draw(background);
    window.draw(panel);

    window.draw(titleText);
    window.draw(scoreText);
    window.draw(promptText);
    window.draw(nameInputText);
    window.draw(infoText);

    window.display();
}