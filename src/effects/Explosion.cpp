#include "Explosion.h"

Explosion::Explosion(sf::Vector2f position, const sf::Texture &texture, sf::Vector2i fSize, int nFrames, float duration)
    : frameSize(fSize),
      numFrames(nFrames),
      currentFrame(0),
      frameDuration(duration / nFrames),
      elapsedTime(0.f),
      finished(false)
{
    sprite.emplace(texture);

    setPosition(position);
    setOrigin({frameSize.x / 2.f, frameSize.y / 2.f});

    cols = texture.getSize().x / frameSize.x;

    sprite->setTextureRect(sf::IntRect({0, 0}, frameSize));
}

void Explosion::update(float deltaTime)
{
    if (finished)
        return;

    elapsedTime += deltaTime;

    if (elapsedTime >= frameDuration)
    {
        elapsedTime -= frameDuration;
        currentFrame++;

        if (currentFrame >= numFrames)
        {
            finished = true;
        }
        else
        {
            int col = currentFrame % cols;
            int row = currentFrame / cols;

            sf::IntRect rect({col * frameSize.x, row * frameSize.y}, frameSize);

            if (sprite)
                sprite->setTextureRect(rect);
        }
    }
}

void Explosion::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (sprite)
    {
        states.transform *= getTransform();
        target.draw(*sprite, states);
    }
}