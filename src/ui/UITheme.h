#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class UITheme
{
private:
    sf::Font titleFont;
    sf::Font bodyFont;

    // Constructor privado para el Singleton
    UITheme();

public:
    // Acceso a la instancia unica
    static UITheme &getInstance();

    // Eliminar constructores de copia/asignacion
    UITheme(const UITheme &) = delete;
    void operator=(const UITheme &) = delete;

    // gtt de fuentes
    const sf::Font &getTitleFont() const;
    const sf::Font &getBodyFont() const;
    const sf::Font &getPixelFont() const;

    // Paleta de colores LCARS (Star Trek)
    static const sf::Color LCARS_Gold;
    static const sf::Color LCARS_Orange;
    static const sf::Color LCARS_Periwinkle;
    static const sf::Color LCARS_Red;
    static const sf::Color LCARS_White;
    static const sf::Color LCARS_DarkBorder;

    // Estilos predefinidos (CSS casero)
    static void applyTitleStyle(sf::Text &text);
    static void applyHeaderStyle(sf::Text &text);
    static void applyHighlightStyle(sf::Text &text);
    static void applyBodyStyle(sf::Text &text);

    // Estilos interactivos
    static void applyMenuOptionStyle(sf::Text &text, bool isSelected);
    static void applyNavigationStyle(sf::Text &text, bool isHovered = false);

    // Estilos genericos
    static void applyLabelStyle(sf::Text &text);
    static void applySolidStyle(sf::Text &text, sf::Color color);
};