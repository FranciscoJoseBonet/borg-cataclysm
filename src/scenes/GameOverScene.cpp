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
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);
    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

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

        float scaleX = baseResolution.x / static_cast<float>(texSize.x);
        float scaleY = baseResolution.y / static_cast<float>(texSize.y);

        float maxScale = std::max(scaleX, scaleY);
        background.setScale({maxScale, maxScale});
    }

    panel.setSize({600.f, 500.f});
    panel.setFillColor(sf::Color(0, 0, 0, 180));
    panel.setOutlineColor(sf::Color::White);
    panel.setOutlineThickness(2.f);

    sf::Vector2f pSize = panel.getSize();
    panel.setOrigin({pSize.x / 2.f, pSize.y / 2.f});
    panel.setPosition({baseResolution.x / 2.f, baseResolution.y / 2.f});

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

void GameOverScene::updateView()
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

void GameOverScene::centerText(sf::Text &text, float yOffset)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition({baseResolution.x / 2.f, (baseResolution.y / 2.f) + yOffset});
}

void GameOverScene::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Resized>())
    {
        updateView();
    }

    if (const auto *textEvent = event.getIf<sf::Event::TextEntered>())
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
                switchToScore = true;
            }
        }
        else if (unicode >= 32 && unicode < 128 && playerName.size() < 10)
        {
            playerName += static_cast<char>(unicode);
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
    window.setView(view);

    window.draw(background);
    window.draw(panel);

    window.draw(titleText);
    window.draw(scoreText);
    window.draw(promptText);
    window.draw(nameInputText);
    window.draw(infoText);

    window.display();
}