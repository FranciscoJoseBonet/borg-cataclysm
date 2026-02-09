#include "ResourceManager.h"
#include <iostream>

sf::Texture &ResourceManager::getTexture(const std::string &filepath)
{
    // 1. Buscamos en el cache si ya la cargamos antes
    auto it = textures.find(filepath);

    if (it != textures.end())
    {
        return it->second; // Ya existe, la devolvemos rapido
    }

    // 2. Si no existe, crea una entry vacia en el mapa y obtenemos su referencia
    sf::Texture &tex = textures[filepath];

    // 3. Intentamos cargar desde disco
    if (!tex.loadFromFile(filepath))
    {
        std::cerr << "ERRORRRRRR No se pudo cargar textura: " << filepath << "\n";

        // 4. Aca usamos el fallback. Si falla, creamos un cuadrado Magenta (leí que era lo normal de hacer)
        // Asi el juego no crashea y vemos un cuadrado rosa donde falta la imagen
        sf::Image fallback;
        fallback.resize({32, 32}, sf::Color::Magenta);

        if (!tex.loadFromImage(fallback))
        {
            std::cerr << "Error, falló la carga de textura fallback, que raroo\n";
        }
    }

    return tex;
}

void ResourceManager::clear()
{
    textures.clear();
}