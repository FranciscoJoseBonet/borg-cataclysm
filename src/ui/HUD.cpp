#include "HUD.h"
#include "UITheme.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

HUD::HUD()
    : scoreText(UITheme::getInstance().getTitleFont()),
      levelText(UITheme::getInstance().getTitleFont()),
      levelTextTimer(0.f)
{
}

void HUD::init(const sf::Vector2f &res, ResourceManager &resources)
{
    baseResolution = res;

    // 1. Configurar Textos
    UITheme::applyTitleStyle(scoreText);
    scoreText.setCharacterSize(40);
    scoreText.setPosition({baseResolution.x - 20.f, 30.f});

    UITheme::applyTitleStyle(levelText);
    levelText.setCharacterSize(100);
    levelText.setFillColor(sf::Color(255, 255, 255, 0));

    // 2. Configurar Iconos (Usamos emplace para construir el sprite dentro del optional, esto es de SFML 3.0.0!!!!!!!!!)

    // Icono Salud
    const sf::Texture &healTex = resources.getTexture("assets/img/PowerUps/PU_Heal.png");
    healthIcon.emplace(healTex); // Construccion diferida
    float iconScaleH = 25.f / healTex.getSize().y;
    healthIcon->setScale({iconScaleH, iconScaleH}); // Laflechita es porque tenemos el puntero a donde esta el sprite, no el sprite (lo cargamos con optional)
    healthIcon->setPosition({20.f, 20.f});

    // Icono Escudo
    const sf::Texture &shieldTex = resources.getTexture("assets/img/PowerUps/PU_Extra_Shield.png");
    shieldIcon.emplace(shieldTex);
    float iconScaleS = 25.f / shieldTex.getSize().y;
    shieldIcon->setScale({iconScaleS, iconScaleS});
    shieldIcon->setPosition({20.f, 55.f});

    // 3. Configurar Barras
    float barX = 60.f;

    // Barra de Vida
    healthBarBack.setSize({widthBar, heightBar});
    healthBarBack.setFillColor(UITheme::LCARS_DarkBorder);
    healthBarBack.setPosition({barX, 22.f});
    healthBarBack.setOutlineColor(UITheme::LCARS_Red);
    healthBarBack.setOutlineThickness(1.f);

    healthBar.setSize({widthBar, heightBar});
    healthBar.setFillColor(UITheme::LCARS_Red);
    healthBar.setPosition({barX, 22.f});

    // Barra de Escudo
    shieldBarBack.setSize({widthBar, heightBar});
    shieldBarBack.setFillColor(UITheme::LCARS_DarkBorder);
    shieldBarBack.setPosition({barX, 57.f});
    shieldBarBack.setOutlineColor(UITheme::LCARS_Periwinkle);
    shieldBarBack.setOutlineThickness(1.f);

    shieldBar.setSize({widthBar, heightBar});
    shieldBar.setFillColor(UITheme::LCARS_Periwinkle);
    shieldBar.setPosition({barX, 57.f});

    // 4. Icono Vidas
    // Aca podriamos hacerlo dinámico en la prox act *****
    const sf::Texture &shipTex = resources.getTexture("assets/img/Ships/enterprise-001.PNG");
    shipLifeIcon.emplace(shipTex);
    float shipScale = 30.f / shipTex.getSize().x;
    shipLifeIcon->setScale({shipScale, shipScale});

    // 5. Iconos PowerUps
    auto setupIcon = [&](std::string name, std::string path)
    {
        const sf::Texture &tex = resources.getTexture(path);
        // Aqui creamos un sprite temporal y lo movemos al mapa.
        // El mapa gestiona su propia memoria, no necesita optional si insertamos valor completo de la ruta
        sf::Sprite s(tex);
        float targetSize = 40.f;
        float scale = targetSize / tex.getSize().x;
        s.setScale({scale, scale});

        powerUpIcons.insert_or_assign(name, s);
    };

    setupIcon("DOUBLE", "assets/img/PowerUps/PU_Double_Shot.png");
    setupIcon("RAPID", "assets/img/PowerUps/PU_Rapid_Fire.png");
    setupIcon("INVUL", "assets/img/PowerUps/PU_Invencibility.png");
}

// Esto muestra el NIVEL X cada vez que pasamos de nivel
void HUD::showLevelMessage(int level)
{
    levelText.setString("LEVEL " + std::to_string(level));
    auto bounds = levelText.getLocalBounds();
    levelText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    levelText.setPosition({baseResolution.x / 2.f, baseResolution.y / 3.f});
    levelTextTimer = 3.0f;
}

void HUD::update(float dt)
{
    if (levelTextTimer > 0.f)
    {
        levelTextTimer -= dt;
        sf::Color c = UITheme::LCARS_Gold;

        if (levelTextTimer < 1.0f)
        {
            float alpha = (levelTextTimer / 1.0f) * 255.f;
            c.a = static_cast<std::uint8_t>(alpha);
        }
        else
        {
            c.a = 255;
        }

        levelText.setFillColor(c);
        sf::Color outline = sf::Color(60, 40, 0);
        outline.a = c.a;
        levelText.setOutlineColor(outline);
    }
}

// Actualizacion de stats durante el game
void HUD::updateStats(int score, int lives, float hp, float maxHp, float shield, float maxShield)
{
    std::stringstream ss;
    ss << "SCORE: " << std::setfill('0') << std::setw(6) << score;
    scoreText.setString(ss.str());

    auto bounds = scoreText.getLocalBounds();
    scoreText.setOrigin({bounds.size.x, 0.f});
    scoreText.setPosition({baseResolution.x - 20.f, 20.f});

    currentLives = lives;

    float hpPercent = (maxHp > 0.f) ? (hp / maxHp) : 0.f;
    hpPercent = std::max(0.f, std::min(1.f, hpPercent));
    healthBar.setSize({widthBar * hpPercent, heightBar});

    float shieldPercent = (maxShield > 0.f) ? (shield / maxShield) : 0.f;
    shieldPercent = std::max(0.f, std::min(1.f, shieldPercent));
    shieldBar.setSize({widthBar * shieldPercent, heightBar});
}

// Muestra los powerups en el hud cuando los tiene activos la nave
void HUD::updatePowerUps(bool doubleShot, bool rapidFire, bool invulnerable)
{
    for (auto &pair : powerUpIcons)
    {
        pair.second.setPosition({-100.f, -100.f});
    }

    float startX = 20.f;
    float startY = baseResolution.y - 60.f;
    float spacing = 50.f;
    int index = 0;

    auto placeIfActive = [&](std::string name, bool active)
    {
        if (active && powerUpIcons.count(name))
        {
            powerUpIcons.at(name).setPosition({startX + (index * spacing), startY});
            index++;
        }
    };

    placeIfActive("DOUBLE", doubleShot);
    placeIfActive("RAPID", rapidFire);
    placeIfActive("INVUL", invulnerable);
}

void HUD::draw(sf::RenderTarget &target)
{
    target.draw(scoreText);
    target.draw(healthBarBack);
    target.draw(healthBar);
    target.draw(shieldBarBack);
    target.draw(shieldBar);

    // Dibujamos solo si el optional tiene valor
    if (healthIcon)
        target.draw(*healthIcon);
    if (shieldIcon)
        target.draw(*shieldIcon);

    if (levelTextTimer > 0.f)
    {
        target.draw(levelText);
    }

    // Dibujamos vidas
    if (currentLives > 0 && shipLifeIcon)
    {
        float startX = baseResolution.x - 50.f;
        float startY = baseResolution.y - 50.f;
        float spacing = 40.f;

        for (int i = 0; i < currentLives; ++i)
        {
            shipLifeIcon->setPosition({startX - (i * spacing), startY});
            target.draw(*shipLifeIcon);
        }
    }

    for (const auto &pair : powerUpIcons)
    {
        if (pair.second.getPosition().y > 0)
            target.draw(pair.second);
    }
}