#pragma once
#include <SFML/Graphics.hpp>

// Esta clase facilita muchisimo el uso de los spritesheets distintos (capaz habia una libreria pero este es casero)

class Explosion : public sf::Drawable, public sf::Transformable
{
private:
    // Visuales
    sf::Sprite sprite;

    // Animacion
    sf::Vector2i frameSize; // Tamaño de cada cuadrito de la explosion en px (ej 64x64)
    int numFrames;          // Cuantos cuadros tiene la animacion en total
    int currentFrame;       // En cual vamos
    int cols;               // Cuantas columnas tiene la hoja de sprites

    // Tiempos
    float frameDuration;   // Cuanto dura cada cuadro en pantalla
    float elapsedTime;     // Acumulador de tiempo
    bool finished = false; // Flag para borrarla cuando termine

    // Metodo interno de dibujado
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Explosion(sf::Vector2f position, const sf::Texture &texture, sf::Vector2i frameSize, int numFrames, float duration);

    void update(float deltaTime);

    // Getter para saber si el LevelManager debe borrarla
    bool isFinished() const { return finished; }
};