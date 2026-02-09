#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include "../managers/ResourceManager.h"
#include "../ui/UITheme.h"
#include <vector>
#include <optional>

class SelectionScene : public Scene
{
private:
    sf::RenderWindow &window;
    ResourceManager resources; // Gestor de texturas local

    // Vista y resolucion
    sf::View view;
    sf::Vector2f baseResolution;

    // UI
    sf::Text titleText;
    sf::Text shipNameText;
    sf::Text descriptionText;

    sf::Text leftArrow;
    sf::Text rightArrow;

    // Usamos optional para diferir la construccion del sprite
    // hasta que sepamos que nave mostrar
    std::optional<sf::Sprite> shipPreviewSprite;
    std::optional<sf::Sprite> backgroundSprite;

    // Estado de la Escena
    int currentIndex = 0;
    SceneType nextScene = SceneType::None;

    // Metodos Auxiliares
    void updateUI(); // Actualiza textos y sprites al cambiar de nave
    void updateView();

    // Helper para dibujar las barras de estadisticas en el Render
    void drawStatBar(sf::RenderTarget &target, const std::string &label, float value, float maxValue, float yPos, sf::Color color);

public:
    SelectionScene(sf::RenderWindow &window);

    // Implementacion de Scene
    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override { return nextScene; }
};