#pragma once
#include "Scene.h"
#include "../environment/StarField.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class MenuScene : public Scene
{
private:
    sf::RenderWindow &window;

    // Variables para el manejo de resolucion y camara
    sf::View view;
    sf::Vector2f baseResolution;

    // Fondo animado
    StarField stars;

    // UI
    sf::Text titleText;
    std::vector<sf::Text> menuOptions; // Vector para iterar facil los botones

    int selectedItemIndex = -1; // -1 significa que nada esta seleccionado
    SceneType nextScene = SceneType::None;
    float totalTime = 0.f;

    // Metodos auxiliares internos
    void initMenuOptions();
    void centerText(sf::Text &text, float yOffset);
    void updateView();

public:
    MenuScene(sf::RenderWindow &window);

    // Override: Implementacion real de los metodos de Scene
    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override { return nextScene; }
};