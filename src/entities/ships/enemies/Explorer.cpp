#include "Explorer.h"
#include <cmath>
#include <random>

Explorer::Explorer(const sf::Texture &texture, sf::Vector2f startPos)
    : Enemy(texture, 60.f, "Explorer", 1.5f)
{
    initialX = startPos.x;
    setPosition(startPos);

    setSpeed(90.f);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distFreq(1.5f, 3.0f);
    frequency = distFreq(gen);

    std::uniform_real_distribution<float> distAmp(80.f, 150.f);
    amplitude = distAmp(gen);

    std::uniform_real_distribution<float> distPhase(0.f, 6.28318f);
    startPhase = distPhase(gen);

    if (sprite)
    {
        sprite->setScale({0.4f, 0.4f});
    }
}

void Explorer::update(float dt)
{
    timeAlive += dt;
    Enemy::update(dt);
}

void Explorer::movePattern(float dt)
{
    float oscillation = std::sin((timeAlive * frequency) + startPhase) * amplitude;
    float newX = initialX + oscillation;

    float newY = getPosition().y + (getSpeed() * dt);

    setPosition({newX, newY});

    if (sprite)
    {
        float direction = std::cos((timeAlive * frequency) + startPhase);

        float tilt = direction * 15.f;
        sprite->setRotation(sf::degrees(180.f - tilt));
    }
}