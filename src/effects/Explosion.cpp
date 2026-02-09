#include "Explosion.h"

Explosion::Explosion(sf::Vector2f position, const sf::Texture &texture, sf::Vector2i fSize, int nFrames, float duration)
    : sprite(texture),
      frameSize(fSize),
      numFrames(nFrames),
      currentFrame(0),
      frameDuration(duration / nFrames), // Calculamos cuanto dura cada frame individualmente
      elapsedTime(0.f),
      finished(false)
{
    setPosition(position);

    // 1. Configuracion del Sprite Sheet
    // Calculamos cuantas columnas tiene la imagen dividiendo el ancho total por el ancho de un frame
    cols = texture.getSize().x / frameSize.x;

    // 2. Configuracion Visual
    // Seleccionamos el primer cuadro (0,0)
    sprite.setTextureRect(sf::IntRect({0, 0}, frameSize));

    // Centramos el origen del sprite
    sprite.setOrigin({frameSize.x / 2.f, frameSize.y / 2.f});
}

void Explosion::update(float deltaTime)
{
    if (finished)
        return;

    elapsedTime += deltaTime;

    // Si paso suficiente tiempo, avanzamos al siguiente cuadro
    if (elapsedTime >= frameDuration)
    {
        elapsedTime -= frameDuration; // Llevamos la cuentad el tiempo
        currentFrame++;

        if (currentFrame >= numFrames)
        {
            finished = true; // La animacion termino
        }
        else
        {
            // Calculo de coordenadas en la grilla
            // por ej, si tengo 4 columnas y voy por el frame 5:
            // columna = 5 % 4 = 1
            // fila    = 5 / 4 = 1
            int col = currentFrame % cols;
            int row = currentFrame / cols;

            // Recortamos el rectangulo que toca de la textura
            sf::IntRect rect({col * frameSize.x, row * frameSize.y}, frameSize);
            sprite.setTextureRect(rect);
        }
    }
}

void Explosion::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Aplicamos la transformacion (posicion) de la explosion al estado de render
    states.transform *= getTransform();
    target.draw(sprite, states);
}