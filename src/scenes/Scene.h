#pragma once
#include <SFML/Graphics.hpp>

enum class SceneType
{
    None,
    Menu,
    Game,
    Score,
    GameOver,
    Options,
    Selection
};

// Clase base abstracta para las Scenes (Interfaces)
class Scene
{
public:
    // Destructor virtual (llama al destructor de la clase hija)
    virtual ~Scene() = default;

    // Metodos virtuales puros (= 0) ya que cada clase hija debería gestionar sus eventos y grafios
    virtual void handleEvent(const sf::Event &event) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual SceneType getNextScene() const = 0;

    // Para el uso en ScoreScene o GameOverScene y recuperar la puntuacion de la session
    virtual int getScore() const { return 0; }
};