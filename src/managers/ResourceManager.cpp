#include "ResourceManager.h"
#include <iostream>

sf::Texture &ResourceManager::getTexture(const std::string &filepath)
{

    auto it = textures.find(filepath);

    if (it != textures.end())
    {
        return it->second;
    }

    sf::Texture &tex = textures[filepath];
    if (!tex.loadFromFile(filepath))
    {
        std::cerr << "!! ERROR: No se pudo cargar textura: " << filepath << "\n";

        sf::Image fallback;
        fallback.resize({32, 32}, sf::Color::Magenta);

        if (!tex.loadFromImage(fallback))
        {
            std::cerr << "Error crítico: Falló la carga de textura fallback\n";
        }
    }

    return tex;
}

void ResourceManager::clear()
{
    textures.clear();
}