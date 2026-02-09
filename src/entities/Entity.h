#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Enum para el sistema de bandos de amigos o enemiess
enum class Faction
{
    Player,
    Alien,
    Neutral
};

// Clase Base Abstracta, padre de la mayoria de elementos visibles con textura y hitbox
// Hereda de Drawable (para poder ser dibujada) y Transformable (para tener posicion, rotacion y escala)
class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity(float startHealth, const std::string &type, Faction faction)
        : health(startHealth), type(type), faction(faction), alive(true) {}

    virtual ~Entity() = default;

    // Parte abstracta
    // Cada hijo define su comportamiento (moverse, animarse, etc.)
    virtual void update(float deltaTime) = 0;

    // Necesario para las colisiones, cada hijo elige cual es su hitbox
    virtual sf::FloatRect getBounds() const = 0;

    // Logica del juego
    virtual void takeDamage(float amount)
    {
        health -= amount;
        if (health <= 0 && alive)
            destroy();
    }

    // Marca la entidad para ser borrada en el siguiente frame
    virtual void destroy() { alive = false; }

    // Gtt y Stt

    float getHealth() const { return health; }
    void setHealth(float h) { health = h; }

    bool isAlive() const { return alive; }

    Faction getFaction() const { return faction; }
    std::string getType() const { return type; }

protected:
    // Variables accesibles por las clases hijas (Naves, Balas)
    float health;
    std::string type; // Identificador util para debug o logica especifica interna que usamos en managers
    Faction faction;
    bool alive;

    // Metodo interno de SFML
    // Es protegido porque nadie llama a entity.draw(), lo llama la ventana
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};