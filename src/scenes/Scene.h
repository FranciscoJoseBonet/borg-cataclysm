#ifndef SCENE_H
#define SCENE_H

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "Entity.h"

class Scene
{
public:
    Scene();
    virtual ~Scene();

    virtual void update(float frameTime);
    virtual void render(sf::RenderWindow &window);
    virtual void handleEvent(const sf::Event &event);

    void add(Entity *entity);
    void remove(Entity *entity);

private:
    std::vector<Entity *> entities;
};

#endif
