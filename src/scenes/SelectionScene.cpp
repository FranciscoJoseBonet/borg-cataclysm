#include "SelectionScene.h"
#include "../data/ShipRepository.h"
#include "../core/GameSession.h"
#include <iostream>
#include <cmath>

SelectionScene::SelectionScene(sf::RenderWindow &w)
    : window(w),

      titleText(UITheme::getInstance().getTitleFont()),
      shipNameText(UITheme::getInstance().getTitleFont()),
      leftArrow(UITheme::getInstance().getTitleFont()),
      rightArrow(UITheme::getInstance().getTitleFont()),

      descriptionText(UITheme::getInstance().getBodyFont())
{
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);
    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    // TITULO
    titleText.setString("SELECCIONAR NAVE");
    UITheme::applyTitleStyle(titleText);

    auto bounds = titleText.getLocalBounds();
    titleText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    titleText.setPosition({baseResolution.x / 2.f, baseResolution.y * 0.08f});

    shipNameText.setCharacterSize(54);
    UITheme::applyHighlightStyle(shipNameText);

    descriptionText.setCharacterSize(26);
    UITheme::applyBodyStyle(descriptionText);

    leftArrow.setString("<");
    leftArrow.setCharacterSize(96);
    UITheme::applyNavigationStyle(leftArrow);

    rightArrow.setString(">");
    rightArrow.setCharacterSize(96);
    UITheme::applyNavigationStyle(rightArrow);

    currentIndex = 0;
    updateUI();
}

void SelectionScene::updateUI()
{
    const auto &ships = ShipRepository::getAllShips();
    const auto &data = ships[currentIndex];

    // Fondo
    const sf::Texture &bgTex = resources.getTexture(data.planetBackgroundPath);
    backgroundSprite.emplace(bgTex);
    float scale = (baseResolution.y * 0.7f) / bgTex.getSize().y;
    backgroundSprite->setScale({scale, scale});
    auto bgBounds = backgroundSprite->getLocalBounds();
    backgroundSprite->setOrigin({bgBounds.size.x / 2.f, bgBounds.size.y / 2.f});
    backgroundSprite->setPosition({baseResolution.x / 2.f, baseResolution.y * 0.45f});

    // Nave
    const sf::Texture &shipTex = resources.getTexture(data.texturePath);
    shipPreviewSprite.emplace(shipTex);
    auto shipBounds = shipPreviewSprite->getLocalBounds();
    shipPreviewSprite->setOrigin({shipBounds.size.x / 2.f, shipBounds.size.y / 2.f});
    shipPreviewSprite->setPosition({baseResolution.x / 2.f, baseResolution.y * 0.45f});
    shipPreviewSprite->setScale({2.5f, 2.5f});

    // Textos
    shipNameText.setString(data.name);
    auto nameBounds = shipNameText.getLocalBounds();
    shipNameText.setOrigin({nameBounds.size.x / 2.f, nameBounds.size.y / 2.f});
    shipNameText.setPosition({baseResolution.x / 2.f, baseResolution.y * 0.18f});

    descriptionText.setString(data.description);
    auto descBounds = descriptionText.getLocalBounds();
    descriptionText.setOrigin({descBounds.size.x / 2.f, descBounds.size.y / 2.f});
    descriptionText.setPosition({baseResolution.x / 2.f, baseResolution.y * 0.90f});

    // Flechas
    leftArrow.setPosition({baseResolution.x * 0.1f, baseResolution.y * 0.45f - 40.f});
    rightArrow.setPosition({baseResolution.x * 0.9f - 40.f, baseResolution.y * 0.45f - 40.f});
}

void SelectionScene::updateView()
{
    float windowRatio = (float)window.getSize().x / (float)window.getSize().y;
    float viewRatio = baseResolution.x / baseResolution.y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    if (horizontalSpacing)
    {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    }
    else
    {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
}

void SelectionScene::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Resized>())
    {
        updateView();
    }

    int totalShips = static_cast<int>(ShipRepository::getAllShips().size());

    if (const auto *keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyEvent->code == sf::Keyboard::Key::Left || keyEvent->code == sf::Keyboard::Key::A)
        {
            currentIndex = (currentIndex - 1 + totalShips) % totalShips;
            updateUI();
        }
        else if (keyEvent->code == sf::Keyboard::Key::Right || keyEvent->code == sf::Keyboard::Key::D)
        {
            currentIndex = (currentIndex + 1) % totalShips;
            updateUI();
        }
        else if (keyEvent->code == sf::Keyboard::Key::Enter)
        {
            GameSession::selectedShipIndex = currentIndex;
            nextScene = SceneType::Game;
        }
        else if (keyEvent->code == sf::Keyboard::Key::Escape)
        {
            nextScene = SceneType::Menu;
        }
    }

    if (const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
            if (leftArrow.getGlobalBounds().contains(mousePos))
            {
                currentIndex = (currentIndex - 1 + totalShips) % totalShips;
                updateUI();
            }
            else if (rightArrow.getGlobalBounds().contains(mousePos))
            {
                currentIndex = (currentIndex + 1) % totalShips;
                updateUI();
            }
        }
    }
}

void SelectionScene::update()
{
    static float time = 0.f;
    time += 0.04f;
    float offset = std::sin(time) * 8.f;

    if (shipPreviewSprite)
    {
        float baseY = baseResolution.y * 0.45f;
        shipPreviewSprite->setPosition({baseResolution.x / 2.f, baseY + offset});
    }

    if (backgroundSprite)
    {
        backgroundSprite->rotate(sf::degrees(0.01f));
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
    UITheme::applyNavigationStyle(leftArrow, leftArrow.getGlobalBounds().contains(mousePos));
    UITheme::applyNavigationStyle(rightArrow, rightArrow.getGlobalBounds().contains(mousePos));
}

void SelectionScene::drawStatBar(sf::RenderTarget &target, const std::string &label, float value, float maxValue, float yPos, sf::Color color)
{
    float barWidth = baseResolution.x * 0.3f;
    float barHeight = 12.f;
    float centerX = baseResolution.x / 2.f;
    float startX = centerX - (barWidth / 2.f) + 40.f;

    sf::Text labelText(UITheme::getInstance().getBodyFont());
    labelText.setString(label);
    labelText.setCharacterSize(22);
    UITheme::applyLabelStyle(labelText);

    auto labelBounds = labelText.getLocalBounds();
    labelText.setOrigin({labelBounds.size.x, labelBounds.size.y / 2.f});

    labelText.setPosition({startX - 15.f, yPos + (barHeight / 2.f) - 5.f});
    target.draw(labelText);

    sf::RectangleShape bgBar({barWidth, barHeight});
    bgBar.setPosition({startX, yPos});
    bgBar.setFillColor(sf::Color(50, 50, 50, 200));
    bgBar.setOutlineColor(sf::Color::White);
    bgBar.setOutlineThickness(1.f);
    target.draw(bgBar);

    // Barra Relleno
    float percentage = std::min(value / maxValue, 1.0f);
    sf::RectangleShape fillBar({barWidth * percentage, barHeight});
    fillBar.setPosition({startX, yPos});
    fillBar.setFillColor(color);
    target.draw(fillBar);
}

void SelectionScene::render()
{
    window.clear(sf::Color::Black);
    window.setView(view);

    if (backgroundSprite)
        window.draw(*backgroundSprite);
    if (shipPreviewSprite)
        window.draw(*shipPreviewSprite);

    window.draw(titleText);
    window.draw(shipNameText);
    window.draw(descriptionText);
    window.draw(leftArrow);
    window.draw(rightArrow);

    const auto &data = ShipRepository::getShip(currentIndex);

    float startStatsY = baseResolution.y * 0.65f;
    float gap = 30.f;

    drawStatBar(window, "INTEGRIDAD", data.maxHealth, 200.f, startStatsY, sf::Color::Green);
    drawStatBar(window, "VELOCIDAD", data.speed, 400.f, startStatsY + gap, sf::Color::Cyan);

    float dmg = (float)data.primaryWeapon.damage;
    drawStatBar(window, "POTENCIA", dmg, 30.f, startStatsY + gap * 2, sf::Color::Red);

    float fireRate = (data.primaryWeapon.cooldown > 0) ? (1.f / data.primaryWeapon.cooldown) : 0.f;
    drawStatBar(window, "CADENCIA", fireRate, 8.f, startStatsY + gap * 3, sf::Color::Yellow);

    sf::Text enterText(UITheme::getInstance().getBodyFont());
    enterText.setString("[ESC] MENU PRINCIPAL      [ENTER] INICIAR MISION");
    enterText.setCharacterSize(24);
    UITheme::applyLabelStyle(enterText);

    auto b = enterText.getLocalBounds();
    enterText.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    enterText.setPosition({baseResolution.x / 2.f, baseResolution.y * 0.85f});

    static float timer = 0;
    timer += 0.01f;
    if ((int)timer % 2 == 0)
        window.draw(enterText);

    window.display();
}