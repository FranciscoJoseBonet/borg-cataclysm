#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class UITheme
{
private:
    sf::Font titleFont;
    sf::Font bodyFont;
    sf::Font pixelFont;

    UITheme();

public:
    static UITheme &getInstance();

    UITheme(const UITheme &) = delete;
    void operator=(const UITheme &) = delete;

    const sf::Font &getTitleFont() const;
    const sf::Font &getBodyFont() const;

    static const sf::Color LCARS_Gold;
    static const sf::Color LCARS_Orange;
    static const sf::Color LCARS_Periwinkle;
    static const sf::Color LCARS_Red;
    static const sf::Color LCARS_White;
    static const sf::Color LCARS_DarkBorder;

    static void applyTitleStyle(sf::Text &text);
    static void applyHeaderStyle(sf::Text &text);
    static void applyHighlightStyle(sf::Text &text);

    static void applyBodyStyle(sf::Text &text);
    static void applyMenuOptionStyle(sf::Text &text, bool isSelected);
    static void applyNavigationStyle(sf::Text &text, bool isHovered = false);
    static void applyLabelStyle(sf::Text &text);
    static void applySolidStyle(sf::Text &text, sf::Color color);
};