#include "UITheme.h"

const sf::Color UITheme::LCARS_Gold(255, 204, 102);
const sf::Color UITheme::LCARS_Orange(255, 153, 51);
const sf::Color UITheme::LCARS_Periwinkle(153, 153, 255);
const sf::Color UITheme::LCARS_Red(204, 68, 68);
const sf::Color UITheme::LCARS_White(255, 255, 255);
const sf::Color UITheme::LCARS_DarkBorder(10, 10, 20);

UITheme::UITheme()
{
    if (!titleFont.openFromFile("../assets/fonts/Star_Trek_Enterprise_Future.ttf"))
    {
        std::cerr << "UITheme: Error cargando Title Font. Usando fallback.\n";

        if (!titleFont.openFromFile("../assets/fonts/Star_Trek.ttf"))
        {
            std::cerr << "UITheme: FATAL - No se pudo cargar la fuente de titulo de respaldo.\n";
        }
    }

    if (!bodyFont.openFromFile("../assets/fonts/Star_Trek_Enterprise.ttf"))
    {
        std::cerr << "UITheme: Error cargando Body Font (Star_Trek_Enterprise.ttf). Usando fallback de titulo.\n";
        if (!bodyFont.openFromFile("../assets/fonts/Star_Trek_Enterprise_Future.ttf"))
        {
            std::cerr << "UITheme: FATAL - No se pudo cargar fallback para body font.\n";
        }
    }
}

UITheme &UITheme::getInstance()
{
    static UITheme instance;
    return instance;
}

const sf::Font &UITheme::getTitleFont() const
{
    return titleFont;
}

const sf::Font &UITheme::getBodyFont() const
{
    return bodyFont;
}

void UITheme::applyTitleStyle(sf::Text &text)
{
    text.setFont(getInstance().getTitleFont());
    text.setFillColor(LCARS_Gold);
    text.setOutlineColor(sf::Color(60, 40, 0));
    text.setOutlineThickness(4.f);
    if (text.getCharacterSize() < 40)
        text.setCharacterSize(60);
}

void UITheme::applyHeaderStyle(sf::Text &text)
{
    text.setFont(getInstance().getTitleFont());
    text.setFillColor(LCARS_White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3.0f);
}

void UITheme::applyHighlightStyle(sf::Text &text)
{
    text.setFont(getInstance().getTitleFont());
    text.setFillColor(LCARS_White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3.5f);
}

void UITheme::applyBodyStyle(sf::Text &text)
{
    text.setFont(getInstance().getBodyFont());
    text.setFillColor(sf::Color(220, 220, 255));
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.5f);
    text.setLineSpacing(1.1f);
}

void UITheme::applyMenuOptionStyle(sf::Text &text, bool isSelected)
{
    text.setFont(getInstance().getBodyFont());
    if (isSelected)
    {
        text.setFillColor(LCARS_Gold);
        text.setOutlineColor(sf::Color(80, 20, 0));
        text.setOutlineThickness(1.5f);
        text.setScale({1.1f, 1.1f});
    }
    else
    {
        text.setFillColor(LCARS_Periwinkle);
        text.setOutlineColor(LCARS_DarkBorder);
        text.setOutlineThickness(1.5f);
        text.setScale({1.0f, 1.0f});
    }
}

void UITheme::applyNavigationStyle(sf::Text &text, bool isHovered)
{
    text.setFont(getInstance().getTitleFont());
    text.setOutlineColor(LCARS_DarkBorder);
    text.setOutlineThickness(3.f);

    if (isHovered)
    {
        text.setFillColor(LCARS_Orange);
        text.setScale({1.2f, 1.2f});
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