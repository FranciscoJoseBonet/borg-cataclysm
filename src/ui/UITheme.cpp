#include "UITheme.h"

// Basados en la paleta oficial de TNG (Okudagrams)
const sf::Color UITheme::LCARS_Gold(255, 204, 102);       // Mostaza/Dorado
const sf::Color UITheme::LCARS_Orange(255, 153, 51);      // Naranja intenso
const sf::Color UITheme::LCARS_Periwinkle(153, 153, 255); // Lavanda azulado
const sf::Color UITheme::LCARS_Red(204, 68, 68);          // Rojo terracota
const sf::Color UITheme::LCARS_DarkBorder(20, 20, 30);    // Casi negro azulado

UITheme::UITheme()
{
    if (!font.openFromFile("../assets/fonts/Star_Trek_Enterprise_Future.ttf"))
    {
        std::cerr << "UITheme: Fallo carga fuente principal, usando pixel fallback.\n";
        if (!font.openFromFile("../assets/fonts/pixel_font.ttf"))
        {
            std::cerr << "UITheme: FATAL - No hay fuentes.\n";
        }
    }
}

UITheme &UITheme::getInstance()
{
    static UITheme instance;
    return instance;
}

const sf::Font &UITheme::getFont() const
{
    return font;
}

void UITheme::applyTitleStyle(sf::Text &text)
{
    text.setFillColor(LCARS_Gold);
    text.setOutlineColor(sf::Color(60, 40, 0)); // Bronce oscuro
    text.setOutlineThickness(3.f);
    if (text.getCharacterSize() < 30)
        text.setCharacterSize(60);
}

void UITheme::applyHeaderStyle(sf::Text &text)
{
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.f);
}

void UITheme::applyMenuOptionStyle(sf::Text &text, bool isSelected)
{
    if (isSelected)
    {
        // Estilo Activo (Naranja Alerta)
        text.setFillColor(LCARS_Gold);
        text.setOutlineColor(sf::Color(80, 20, 0));
        text.setOutlineThickness(2.f);
        text.setScale({1.1f, 1.1f});
    }
    else
    {
        text.setFillColor(LCARS_Periwinkle);
        text.setOutlineColor(sf::Color(10, 10, 40));
        text.setOutlineThickness(2.f);
        text.setScale({1.0f, 1.0f});
    }
}

void UITheme::applyBodyStyle(sf::Text &text)
{
    text.setFillColor(sf::Color(220, 220, 230));
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.5f);
    text.setLineSpacing(1.2f);
}