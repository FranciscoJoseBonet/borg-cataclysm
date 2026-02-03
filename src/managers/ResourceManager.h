#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager
{
private:
    std::map<std::string, sf::Texture> textures;

public:
    ResourceManager() = default;

    sf::Texture &getTexture(const std::string &filepath);

    void clear();
};