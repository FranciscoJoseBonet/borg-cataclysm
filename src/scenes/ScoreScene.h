#pragma once
#include "Scene.h"
#include "../ui/UITheme.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Struct para manejar los datos leidos del archivo
struct ScoreEntry
{
    std::string name;
    int points;
};

class ScoreScene : public Scene
{
private:
    sf::RenderWindow &window;

    // Variables de vista y res
    sf::View view;
    sf::Vector2f baseResolution;

    // Assets Graficos para la scene
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::RectangleShape panel; // Fondo semitransparente para la tabla

    // Elementos de UI
    sf::Text titleText;
    sf::Text playAgainButton;
    sf::Text menuButton;

    // Lista dinamica de textos para mostrar los puntajes en pantalla
    std::vector<sf::Text> scoreTexts;

    // Variables de Estado
    float totalTime;
    SceneType nextScene = SceneType::None;

    // Metodos Auxiliares
    void loadAndSortScores(); // Lee, ordena y genera los textos
    void centerText(sf::Text &text, float yOffset);
    void setupButton(sf::Text &text, std::string label, float xOffset, float yOffset);
    void updateView();

public:
    ScoreScene(sf::RenderWindow &window);

    // Implementacion de la Interfaz Scene
    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override { return nextScene; }
};