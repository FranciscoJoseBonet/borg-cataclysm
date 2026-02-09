#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager
{
private:
    // Cache de texturas usando un diccionario
    // Clave: String (ruta del archivo) y el valor: Texture (imagen en VRAM)
    std::map<std::string, sf::Texture> textures;

public:
    ResourceManager() = default;

    // Devuelve la ref a la textura, es la utilidad principal de la clase digamos
    // Si no existe, la carga. Si ya existe, la devuelve del cache.
    // Retorna referencia (&)
    sf::Texture &getTexture(const std::string &filepath);

    // Limpia la memoria de video
    void clear();
};