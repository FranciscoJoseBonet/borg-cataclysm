#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class UITheme
{
private:
    sf::Font font;
    sf::Font pixelFont;

    UITheme();

public:
    // Acceso a la instancia única
    static UITheme &getInstance();

    UITheme(const UITheme &) = delete;
    void operator=(const UITheme &) = delete;

    const sf::Font &getFont() const;

    static const sf::Color LCARS_Gold;       // Títulos
    static const sf::Color LCARS_Orange;     // Selección / Alerta
    static const sf::Color LCARS_Periwinkle; // Texto estándar / Ciencias
    static const sf::Color LCARS_Red;        // Errores / Enemigos
    static const sf::Color LCARS_DarkBorder; // Bordes oscuros

    // Aplica estilo de Título Principal (Grande, Dorado)
    static void applyTitleStyle(sf::Text &text);

    // Aplica estilo de Subtítulo o Cabecera (Mediano, Blanco/Periwinkle)
    static void applyHeaderStyle(sf::Text &text);

    // Aplica estilo de Texto de Menú (Normal o Seleccionado)
    static void applyMenuOptionStyle(sf::Text &text, bool isSelected);

    // Aplica estilo para descripciones (Pequeño, legible)
    static void applyBodyStyle(sf::Text &text);
};