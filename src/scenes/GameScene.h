#pragma once
#include <random>
#include "Scene.h"
#include "../ui/UITheme.h"
#include "../environment/StarField.h"
#include "../entities/ships/SpaceShip.h"
#include "../managers/EntityManager.h"
#include "../managers/ResourceManager.h"
#include "../managers/CollisionManager.h"
#include "../managers/LevelManager.h"
#include "../effects/Explosion.h"
#include "../ui/HUD.h"

class GameScene : public Scene
{
private:
    sf::RenderWindow &window;
    std::mt19937 rng;
    sf::Clock deltaClock;

    // Los manager gestionan lógicas internas
    ResourceManager resources;
    EntityManager manager;
    CollisionManager collisionManager;
    std::unique_ptr<LevelManager> levelManager; // Unique ptr porque el nivel puede reiniciarse

    // Vistas
    sf::View view;    // Camara del juego
    sf::View hudView; // Camara de la UI
    sf::Vector2f baseResolution;

    // Entidades y ambiente
    StarField stars;
    SpaceShip *player = nullptr;                        // Referencia rapida al jugador (el manager es el dueño real, aca lo usamos de conexion)
    std::vector<std::unique_ptr<Explosion>> explosions; // Efectos visuales temporales (no están todos terminados los de enemy todavia)

    // UI y los estados que usamos para operar
    HUD hud;
    bool isPaused = false;
    bool gameOver = false;
    int score = 0;
    SceneType nextScene = SceneType::None;

    // Elementos del Menu de Pausa
    sf::RectangleShape pauseOverlay;
    sf::Text pauseTitle;
    sf::Text resumeButton;
    sf::Text menuButton;

    // Metodos auxiliares
    void initPauseMenu();
    void updateView();

public:
    GameScene(sf::RenderWindow &window);

    // Ciclo de vida de la escena (el que heredamos de scene)
    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override
    {
        if (gameOver)
            return SceneType::GameOver;
        return nextScene;
    }

    // Es por el GameOverScene, para saber el puntaje final
    int getScore() const override { return score; }
};