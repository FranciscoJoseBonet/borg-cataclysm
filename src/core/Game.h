#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../scenes/Scene.h"

class Game
{
private:
    // --- Variables de Estado ---
    sf::RenderWindow window;
    std::unique_ptr<Scene> currentScene; // Puntero inteligente para manejo polimorfico de escenas
    bool isFullscreen;

    // --- Métodos Auxiliares ---
    void toggleFullscreen();

public:
    Game();

    // Bucle principal del juego
    void run();

    // Getter necesario para pasar la ventana a las escenas si hace falta
    sf::RenderWindow &getWindow() { return window; }
};