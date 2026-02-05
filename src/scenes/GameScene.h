#pragma once
#include <random>
#include "Scene.h"
#include "../environment/StarField.h"
#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Scout.h"
#include "../entities/items/PowerUp.h"
#include "../managers/EntityManager.h"
#include "../managers/ResourceManager.h"
#include "../managers/CollisionManager.h"
#include "../ui/HUD.h"

class GameScene : public Scene
{
private:
    sf::RenderWindow &window;
    ResourceManager resources;
    EntityManager manager;
    CollisionManager collisionManager;
    std::mt19937 rng;

    sf::View view;
    sf::View hudView;
    sf::Vector2f baseResolution;

    HUD hud;

    sf::Clock deltaClock;
    StarField stars;

    SpaceShip *player = nullptr;
    bool gameOver = false;
    int score = 0;

    bool isPaused = false;
    sf::Font font;
    sf::RectangleShape pauseOverlay;
    sf::Text pauseTitle;
    sf::Text resumeButton;
    sf::Text menuButton;

    SceneType nextScene = SceneType::None;

    void spawnEnemyWave(int count);
    void spawnPowerUp(sf::Vector2f position);
    void initPauseMenu();
    void updateView();

public:
    GameScene(sf::RenderWindow &window);

    void handleEvent(const sf::Event &event) override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override
    {
        if (gameOver)
            return SceneType::GameOver;
        return nextScene;
    }

    int getScore() const override { return score; }
};