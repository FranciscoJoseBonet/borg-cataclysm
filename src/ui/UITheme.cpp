#include "UITheme.h"

// Paleta de colores
const sf::Color UITheme::LCARS_Gold(255, 204, 102);
const sf::Color UITheme::LCARS_Orange(255, 153, 51);
const sf::Color UITheme::LCARS_Periwinkle(153, 153, 255);
const sf::Color UITheme::LCARS_Red(204, 68, 68);
const sf::Color UITheme::LCARS_White(255, 255, 255);
const sf::Color UITheme::LCARS_DarkBorder(10, 10, 20);

// En el constructor cargamos los recursos
UITheme::UITheme()
{
    // 1. Cargar Title Font
    if (!titleFont.openFromFile("assets/fonts/Star_Trek_Enterprise_Future.ttf"))
    {
        std::cerr << "[UITheme] ERROR: No se cargo Title Font. Probando fallback...\n";
        if (!titleFont.openFromFile("assets/fonts/Star_Trek.ttf")) // Fallback por si pasa algo con las fuentes principales
                                                                   //(lo usamos para catchear el warning y evitar que crashee porque falte la fuente)
        {
            std::cerr << "[UITheme] FATAL: No hay fuentes disponibles.\n";
        }
    }

    // 2. Cargar Body Font
    if (!bodyFont.openFromFile("assets/fonts/Star_Trek_Enterprise.ttf"))
    {
        std::cerr << "[UITheme] ERROR: No se cargo Body Font. Usando Title Font como respaldo.\n";
        bodyFont = titleFont; // Fallback si se cargo la anterior para no repetir codigo (DRY), a lo sumo se verá feo
    }
}

// Singleton
UITheme &UITheme::getInstance()
{
    static UITheme instance;
    return instance;
}

// Gtt
const sf::Font &UITheme::getTitleFont() const { return titleFont; }
const sf::Font &UITheme::getBodyFont() const { return bodyFont; }

// aplicadores de estilos (metodos del singleton)

void UITheme::applyTitleStyle(sf::Text &text)
{
    text.setFont(getInstance().getTitleFont());
    text.setFillColor(LCARS_Gold);
    text.setOutlineColor(sf::Color(60, 40, 0));
    text.setOutlineThickness(2.f);

    // Solo forzamos tamaño si no se configuro
    if (text.getCharacterSize() < 20)
        text.setCharacterSize(60);
}

void UITheme::applyHeaderStyle(sf::Text &text)
{
    text.setFont(getInstance().getTitleFont());
    text.setFillColor(LCARS_White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);
}

void UITheme::applyHighlightStyle(sf::Text &text)
{
    text.setFont(getInstance().getTitleFont());
    text.setFillColor(LCARS_White);
    text.setOutlineColor(LCARS_Red);
    text.setOutlineThickness(1.5f);
}

void UITheme::applyBodyStyle(sf::Text &text)
{
    text.setFont(getInstance().getBodyFont());
    text.setFillColor(sf::Color(220, 220, 255));
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.0f);
    text.setLineSpacing(1.1f);
}

void UITheme::applyMenuOptionStyle(sf::Text &text, bool isSelected)
{
    text.setFont(getInstance().getBodyFont());

    if (isSelected)
    {
        text.setFillColor(LCARS_Gold);
        text.setOutlineColor(sf::Color(100, 50, 0));
        text.setOutlineThickness(1.0f);
        text.setScale({1.1f, 1.1f});
    }
    else
    {
        text.setFillColor(LCARS_Periwinkle);
        text.setOutlineColor(sf::Color::Transparent);
        text.setOutlineThickness(0.f);
        text.setScale({1.0f, 1.0f});
    }
}

void UITheme::applyNavigationStyle(sf::Text &text, bool isHovered)
{
    text.setFont(getInstance().getTitleFont());
    text.setOutlineColor(LCARS_DarkBorder);
    text.setOutlineThickness(2.f);

    if (isHovered)
    {
        text.setFillColor(LCARS_Orange);
        text.setScale({1.1f, 1.1f});
    }
    else
    {
        text.setFillColor(LCARS_Periwinkle);
        text.setScale({1.0f, 1.0f});
    }
}

void UITheme::applyLabelStyle(sf::Text &text)
{
    text.setFont(getInstance().getBodyFont());
    text.setFillColor(LCARS_White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.0f);

    if (text.getCharacterSize() == 0)
        text.setCharacterSize(20);
}

void UITheme::applySolidStyle(sf::Text &text, sf::Color color)
{
    text.setFont(getInstance().getBodyFont());
    text.setFillColor(color);
    text.setOutlineThickness(0.f);
}