#pragma once
#include <SFML/Graphics.hpp>

enum class SceneType
{
    None,
    Menu,
    Game,
    Score,
    GameOver,
    Options
};

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void handleEvent(const sf::Event &event) = 0;

    virtual void update() = 0;
    virtual void render() = 0;
    virtual SceneType getNextScene() const = 0;

    virtual int getScore() const { return 0; }
};