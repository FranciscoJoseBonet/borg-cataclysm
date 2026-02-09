#pragma once
#include "Scene.h"
#include "../ui/UITheme.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <cmath>

class GameOverScene : public Scene
{
private:
    sf::RenderWindow &window;

    // Cista y resolucion
    sf::View view;
    sf::Vector2f baseResolution;

    // Estado de la Escena
    int score;
    bool saved;                 // Bandera para no guardar muchas veces
    bool switchToScore = false; // Control de transicion
    float totalTime;
    std::string playerName; // Input del usuario

    // Elementos gráficos
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::RectangleShape panel;

    // UI
    sf::Text titleText;
    sf::Text scoreText;
    sf::Text promptText;
    sf::Text nameInputText;
    sf::Text infoText;

    // Metodos auxiliartes
    void saveScoreToFile(); // Persistencia
    void centerText(sf::Text &text, float yOffset);
    void updateView();

public:
    // Constructor que recibe el puntaje final de la partida anterior
    GameOverScene(sf::RenderWindow &window, int finalScore);

    // Implementacion getNextScene
    SceneType getNextScene() const override
    {
        return switchToScore ? SceneType::Score : SceneType::None;
    }

    // Override de metodos del scene
    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;
};