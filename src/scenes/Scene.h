#pragma once
#include <SFML/Graphics.hpp>

enum class SceneType
{
    None,
    Game,
    GameOver,
    Score
};

class Scene
{
public:
    virtual ~Scene() = default;
    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    virtual SceneType getNextScene() const { return SceneType::None; }

    virtual int getScore() const { return 0; }
};