#include "MenuScene.h"
#include "../ui/UITheme.h"
#include <iostream>
#include <cmath>

MenuScene::MenuScene(sf::RenderWindow &w)
    : window(w),
      stars(w.getSize(), 400), // Inicializamos fondo de estrellas
      titleText(UITheme::getInstance().getTitleFont())
{
    // Guardamos la resolucion original para mantener el aspect ratio luego
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);

    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    titleText.setString("BORG CATACLYSM");
    titleText.setCharacterSize(100);
    UITheme::applyTitleStyle(titleText);

    // Centramos el titulo
    centerText(titleText, -200.f);

    initMenuOptions();
}

// --- METODOS AUXILIARES ---

void MenuScene::initMenuOptions()
{
    std::vector<std::string> labels = {
        "INICIAR MISION",     // Para arrancar a jugar
        "REGISTROS DE VUELO", // Para ver los score
        "SALIR AL ESCRITORIO"};

    float startY = 50.f;
    float spacing = 80.f;

    for (size_t i = 0; i < labels.size(); ++i)
    {
        sf::Text text(UITheme::getInstance().getBodyFont());
        text.setString(labels[i]);
        text.setCharacterSize(40);

        UITheme::applyMenuOptionStyle(text, false);

        // Posicionamos cada opcion mas abajo que la anterior
        centerText(text, startY + (i * spacing));

        menuOptions.push_back(text);
    }
}

void MenuScene::centerText(sf::Text &text, float yOffset)
{
    // Para centrar exacto el texto según el viewport
    auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    text.setPosition({baseResolution.x / 2.f,
                      (baseResolution.y / 2.f) + yOffset});
}

void MenuScene::updateView()
{
    // Calculamos si sobra espacio horizontal o vertical y ajusta el Viewport
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

// Metodos del Gameloop virtuales de scene.h

void MenuScene::handleEvent(const sf::Event &event)
{
    // Si el usuario cambia el tamaño de ventana, recalculamos las barras negras para recentrar los elemntos
    if (event.is<sf::Event::Resized>())
    {
        updateView();
    }

    // Input de Mouse
    if (const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            // La accion depende de que boton estemos hovereando
            // Este indice se calcula en el update()
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
    float dt = 1.f / 60.f; // Delta time fijo aproxxx
    totalTime += dt;
    stars.update(dt);

    // Mover el titulo arriba y abajo con Seno, un lindo vfx
    float yOffset = std::sin(totalTime * 1.5f) * 10.f;
    centerText(titleText, -200.f + yOffset);

    // La logica del mouse y el view
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // Convertimos pixel de pantalla a coordenadas de mundo
    // Esto es necesario porque el View puede tener zoom o espacios vacíos
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos, view);

    selectedItemIndex = -1; // Reset

    for (size_t i = 0; i < menuOptions.size(); ++i)
    {
        // Chequeamos la colision Mouse vs Texto
        if (menuOptions[i].getGlobalBounds().contains(mouseWorld))
        {
            selectedItemIndex = static_cast<int>(i);
            UITheme::applyMenuOptionStyle(menuOptions[i], true); // Estilo Hover
        }
        else
        {
            UITheme::applyMenuOptionStyle(menuOptions[i], false); // Estilo Normal
        }
    }
}

void MenuScene::render()
{
    // Limpiamos pantalla
    window.clear(sf::Color::Black);

    // Aplicamos la camara calculada
    window.setView(view);

    // Dibujamos en orden Fondo, el Titulo y despues UI
    stars.draw(window);
    window.draw(titleText);

    for (const auto &text : menuOptions)
    {
        window.draw(text);
    }

    window.display();
}