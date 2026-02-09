#include "GameOverScene.h"
#include <iostream>
#include <filesystem>
#include <cstdint>

GameOverScene::GameOverScene(sf::RenderWindow &w, int finalScore)
    : window(w),
      score(finalScore),
      saved(false),
      background(backgroundTexture),
      // Inicializacion de textos usando el Singleton de fuentes (el css inventado)
      titleText(UITheme::getInstance().getTitleFont()),
      scoreText(UITheme::getInstance().getBodyFont()),
      promptText(UITheme::getInstance().getBodyFont()),
      nameInputText(UITheme::getInstance().getBodyFont()),
      infoText(UITheme::getInstance().getBodyFont()),
      totalTime(0.f)
{
    // Configuracion de resolucion virtual
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);
    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    // Cargamos el fondo
    if (!backgroundTexture.loadFromFile("assets/img/GAMEOVER_BG.jpg"))
    {
        std::cerr << "Warning: No background texture found.\n";
    }
    else
    {
        background.setTexture(backgroundTexture, true);

        sf::Vector2u texSize = backgroundTexture.getSize();

        // Calculo de escala para que la imagen cubra toda la pantalla (Fill)
        // Usamos static_cast para evitar division entera, lo uso de conversion para evitar el error o warning
        float scaleX = baseResolution.x / static_cast<float>(texSize.x);
        float scaleY = baseResolution.y / static_cast<float>(texSize.y);

        float maxScale = std::max(scaleX, scaleY);
        background.setScale({maxScale, maxScale});
    }

    // Panel semitransparente central
    panel.setSize({600.f, 500.f});
    panel.setFillColor(sf::Color(0, 0, 0, 220));
    panel.setOutlineColor(sf::Color::White);
    panel.setOutlineThickness(2.f);

    sf::Vector2f pSize = panel.getSize();
    panel.setOrigin({pSize.x / 2.f, pSize.y / 2.f});
    panel.setPosition({baseResolution.x / 2.f, baseResolution.y / 2.f});

    // Configuracion de textos
    titleText.setString("GAME OVER");
    UITheme::applyTitleStyle(titleText);
    titleText.setFillColor(UITheme::LCARS_Red); // Rojo de alerta
    centerText(titleText, -220.f);

    scoreText.setString("PUNTUACION: " + std::to_string(score));
    scoreText.setCharacterSize(40);
    UITheme::applyHeaderStyle(scoreText);
    centerText(scoreText, -100.f);

    promptText.setString("INGRESE SU NOMBRE CADETE:");
    promptText.setCharacterSize(30);
    UITheme::applyBodyStyle(promptText);
    promptText.setFillColor(UITheme::LCARS_Gold);
    centerText(promptText, 0.f);

    nameInputText.setString("_");
    nameInputText.setCharacterSize(50);
    UITheme::applyHighlightStyle(nameInputText);
    nameInputText.setFillColor(UITheme::LCARS_Orange);
    centerText(nameInputText, 60.f);

    infoText.setString("[ ENTER PARA GUARDAR - EL KOBAYASHI MARU ERA MAS FACIL... ]");
    infoText.setCharacterSize(18);
    UITheme::applyBodyStyle(infoText);
    centerText(infoText, 200.f);
}

// Metodos auxiliares

void GameOverScene::saveScoreToFile()
{
    // Abrimos en modo append para agregar al final sin borrar lo anterior
    std::ofstream file("scores.txt", std::ios::app);
    if (file.is_open())
    {
        file << playerName << " " << score << "\n";
        file.close();
        std::cout << "Score saved.\n";
    }
}

void GameOverScene::centerText(sf::Text &text, float yOffset)
{
    // Centrado de textos
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition({baseResolution.x / 2.f, (baseResolution.y / 2.f) + yOffset});
}

void GameOverScene::updateView()
{
    // Mantenemos la estructura si se hace pantalla completa
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

// Bueno los metodos de scene necesarios

void GameOverScene::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Resized>())
    {
        updateView();
    }

    // Entrada de texto
    if (const auto *textEvent = event.getIf<sf::Event::TextEntered>())
    {
        uint32_t unicode = textEvent->unicode;

        if (unicode == 8) // Codigo ASCII del Backspace
        {
            if (!playerName.empty())
                playerName.pop_back();
        }
        else if (unicode == 13) // Codigo del Enter
        {
            if (!playerName.empty() && !saved)
            {
                saveScoreToFile();
                saved = true;
                switchToScore = true; // Bandera para cambiar de escena
            }
        }
        // Filtro para caracteres imprimibles (Letras y numeros estandar)
        // Limitamos el nombre a 10 caracteres para que no rompa la UI
        else if (unicode >= 32 && unicode < 128 && playerName.size() < 10)
        {
            playerName += static_cast<char>(unicode);
        }
    }
}

void GameOverScene::update()
{
    float dt = 1.f / 60.f;
    totalTime += dt;

    // Simular cursor parpadeante o fijo agregando un guion bajo
    nameInputText.setString(playerName + "_");
    centerText(nameInputText, 60.f); // Recentrado porque el largo del texto cambio

    // Pequeña animacion del gameover
    float yOffset = std::sin(totalTime * 2.f) * 10.f;
    centerText(titleText, -220.f + yOffset);
}

void GameOverScene::render()
{
    window.clear(sf::Color::Black);
    window.setView(view);

    // Dibujado en capas, primero el fondo despues el panel y despues los textos
    window.draw(background);
    window.draw(panel);

    window.draw(titleText);
    window.draw(scoreText);
    window.draw(promptText);
    window.draw(nameInputText);
    window.draw(infoText);

    window.display();
}