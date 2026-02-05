#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Explosion : public sf::Drawable, public sf::Transformable
{
private:
    std::optional<sf::Sprite> sprite;
    sf::Vector2i frameSize;
    int numFrames;
    int currentFrame;
    float frameDuration;
    float elapsedTime;
    int cols;
    bool finished = false;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Explosion(sf::Vector2f position, const sf::Texture &texture, sf::Vector2i frameSize, int numFrames, float duration);

    void update(float deltaTime);

    bool isFinished() const { return finished; }
};