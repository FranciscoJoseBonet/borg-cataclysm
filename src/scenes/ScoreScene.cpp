#include "ScoreScene.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstdint>

ScoreScene::ScoreScene(sf::RenderWindow &w)
    : window(w),
      background(backgroundTexture), // Inicializamos sprite con la textura
      titleText(UITheme::getInstance().getTitleFont()),
      playAgainButton(UITheme::getInstance().getBodyFont()),
      menuButton(UITheme::getInstance().getBodyFont()),
      totalTime(0.f)
{
    // Configuracion de resolucion virtual
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);
    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    // Carga de Fondo con manejo de errores basico
    if (!backgroundTexture.loadFromFile("assets/img/GAMEOVER_BG.jpg"))
    {
        std::cerr << "Warning: No background texture found.\n";
    }
    else
    {
        background.setTexture(backgroundTexture, true);

        // ajuste a mano para cubrir toda la pantalla manteniendo proporcion (- . -)
        sf::Vector2u texSize = backgroundTexture.getSize();
        float scaleX = baseResolution.x / static_cast<float>(texSize.x);
        float scaleY = baseResolution.y / static_cast<float>(texSize.y);
        float maxScale = std::max(scaleX, scaleY);
        background.setScale({maxScale, maxScale});
    }

    // Panel semitransparente
    panel.setSize({700.f, 650.f});
    panel.setFillColor(sf::Color(0, 0, 0, 220));
    panel.setOutlineColor(UITheme::LCARS_Gold);
    panel.setOutlineThickness(2.f);

    // Centrado del panel
    sf::Vector2f pSize = panel.getSize();
    panel.setOrigin({pSize.x / 2.f, pSize.y / 2.f});
    panel.setPosition({baseResolution.x / 2.f, baseResolution.y / 2.f});

    // Titulo
    titleText.setString("TABLA DE POSICIONES");
    UITheme::applyTitleStyle(titleText);
    centerText(titleText, -280.f);

    // Botones de Navegacion
    setupButton(playAgainButton, "VOLVER A JUGAR", -150.f, 260.f);
    setupButton(menuButton, "MENU PRINCIPAL", 150.f, 260.f);

    // Lectura de archivo y generacion de lista
    loadAndSortScores();
}

// Carga y configuracion

void ScoreScene::loadAndSortScores()
{
    std::vector<ScoreEntry> entries;
    std::ifstream file("scores.txt");

    // 1. Lectura del archivo
    if (file.is_open())
    {
        std::string name;
        int points;
        while (file >> name >> points)
        {
            entries.push_back({name, points});
        }
        file.close();
    }

    // 2. Ordenamiento (lambda para ordenar de mayor a menor)
    std::sort(entries.begin(), entries.end(), [](const ScoreEntry &a, const ScoreEntry &b)
              { return a.points > b.points; });

    // 3. Generacion de Textos UI
    int count = 0;
    float startY = -180.f;
    float spacing = 45.f;

    for (const auto &entry : entries)
    {
        if (count >= 10)
            break; // Solo el Top 10

        // Formato: "Nro. NOMBRE      PUNTOS"
        std::string line = std::to_string(count + 1) + ". " + entry.name;

        // Padding
        while (line.length() < 22)
            line += " ";

        line += std::to_string(entry.points);

        sf::Text text(UITheme::getInstance().getBodyFont());
        text.setString(line);
        text.setCharacterSize(34);

        // Colores para el podio
        sf::Color rankColor;
        if (count == 0)
            rankColor = UITheme::LCARS_Gold; // Oro
        else if (count == 1)
            rankColor = sf::Color(220, 220, 220); // Plata
        else if (count == 2)
            rankColor = sf::Color(205, 127, 50); // Bronce
        else
            rankColor = sf::Color::White;

        UITheme::applySolidStyle(text, rankColor);

        centerText(text, startY + (count * spacing));
        scoreTexts.push_back(text);
        count++;
    }

    // Mensaje si no hay archivo o esta vacio
    if (scoreTexts.empty())
    {
        sf::Text text(UITheme::getInstance().getBodyFont());
        text.setString("NO HAY DATOS AUN");
        text.setCharacterSize(30);
        UITheme::applySolidStyle(text, sf::Color::White);

        centerText(text, 0.f);
        scoreTexts.push_back(text);
    }
}

void ScoreScene::setupButton(sf::Text &text, std::string label, float xOffset, float yOffset)
{
    text.setString(label);
    text.setCharacterSize(28);
    UITheme::applyMenuOptionStyle(text, false);

    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition({(baseResolution.x / 2.f) + xOffset, (baseResolution.y / 2.f) + yOffset});
}

void ScoreScene::centerText(sf::Text &text, float yOffset)
{
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition({baseResolution.x / 2.f, (baseResolution.y / 2.f) + yOffset});
}

void ScoreScene::updateView()
{
    // Logica para mantener aspect ratio cuando redimensionamos
    float windowRatio = (float)window.getSize().x / (float)window.getSize().y;
    float viewRatio = baseResolution.x / baseResolution.y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    if (windowRatio < viewRatio)
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

// Los metodos virtuales del gameloop

void ScoreScene::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Resized>())
        updateView();

    // Input del Mouse
    if (const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            // Mapeamos el click a coordenadas del mundo
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

            if (playAgainButton.getGlobalBounds().contains(mousePos))
                nextScene = SceneType::Game;
            else if (menuButton.getGlobalBounds().contains(mousePos))
                nextScene = SceneType::Menu;
        }
    }
}

void ScoreScene::update()
{
    float dt = 1.f / 60.f;
    totalTime += dt;

    // Animacion suave del titulo
    float yOffset = std::sin(totalTime * 2.f) * 5.f;
    centerText(titleText, -280.f + yOffset);

    // Hover en botones
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

    bool hoverPlay = playAgainButton.getGlobalBounds().contains(mousePos);
    bool hoverMenu = menuButton.getGlobalBounds().contains(mousePos);

    UITheme::applyMenuOptionStyle(playAgainButton, hoverPlay);
    UITheme::applyMenuOptionStyle(menuButton, hoverMenu);
}

void ScoreScene::render()
{
    window.clear(sf::Color::Black);

    // Aplicamos la vista corregida
    window.setView(view);

    window.draw(background);
    window.draw(panel);
    window.draw(titleText);

    // Dibujamos la lista de puntajes
    for (const auto &text : scoreTexts)
        window.draw(text);

    window.draw(playAgainButton);
    window.draw(menuButton);

    window.display();
}