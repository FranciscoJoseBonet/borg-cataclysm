#include "Explorer.h"
#include <cmath>
#include <random>

Explorer::Explorer(const sf::Texture &texture, sf::Vector2f startPos)
    : Enemy(texture, 60.f, "Explorer", 1.5f) // 60 HP, Disparo moderado
{
    initialX = startPos.x;
    setPosition(startPos);
    setSpeed(90.f); // Velocidad vertical

    // Configuracion del rng
    // Lo hacemos static para no crear un generador nuevo por cada nave
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distFreq(1.5f, 3.0f);
    std::uniform_real_distribution<float> distAmp(80.f, 150.f);
    std::uniform_real_distribution<float> distPhase(0.f, 6.28318f); // desde 0 a 2PI

    frequency = distFreq(gen);
    amplitude = distAmp(gen);
    startPhase = distPhase(gen);

    // ajustes visuales (tamaño aca)
    sprite.setScale({0.4f, 0.4f});
}

void Explorer::movePattern(float dt)
{
    // 1. Actualizamos el tiempo de vida
    timeAlive += dt;

    // 2. Calculo de Onda Senoidal, una oscilacion comun seria
    // x = Centro + Seno(tiempo * frecuencia + fase) * amplitud
    float oscillation = std::sin((timeAlive * frequency) + startPhase) * amplitude;

    float newX = initialX + oscillation;
    float newY = getPosition().y + (getSpeed() * dt); // Movimiento constante hacia abajo

    setPosition({newX, newY});

    // 3. Este es un efecto para que la nave cambie el apuntado mientras baja (todavia faltan unos ajustes)
    // use el coseno (derivada del seno) para saber la direccion del giro actual
    float direction = std::cos((timeAlive * frequency) + startPhase);

    // Inclinamos el sprite un poquito
    float tilt = direction * 15.f;

    // Sumamos 180 porque el sprite base mira hacia arriba (o segun tu textura)
    sprite.setRotation(sf::degrees(tilt));
}