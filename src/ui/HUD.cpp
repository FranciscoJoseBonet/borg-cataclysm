#include "HUD.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

HUD::HUD()
    : scoreText(font)
{
}

void HUD::init(const sf::Vector2f &res, ResourceManager &resources)
{
    baseResolution = res;

    if (!font.openFromFile("../assets/fonts/Star_Trek_Enterprise_Future.ttf"))
    {
        if (!font.openFromFile("../assets/fonts/pixel_font.ttf"))
        {
            std::cerr << "No se pudo cargar ninguna fuente en el HUD\n";
        }
    }

    scoreText.setCharacterSize(55);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({baseResolution.x - 20.f, 30.f});

    const sf::Texture &healTex = resources.getTexture("../assets/img/PowerUps/PU_Heal.png");
    healthIcon.emplace(healTex);

    float iconScaleH = 25.f / healTex.getSize().y;
    healthIcon->setScale({iconScaleH, iconScaleH});
    healthIcon->setPosition({20.f, 20.f});

    const sf::Texture &shieldTex = resources.getTexture("../assets/img/PowerUps/PU_Extra_Shield.png");
    shieldIcon.emplace(shieldTex);

    float iconScaleS = 25.f / shieldTex.getSize().y;
    shieldIcon->setScale({iconScaleS, iconScaleS});
    shieldIcon->setPosition({20.f, 55.f});

    float barX = 60.f;

    healthBarBack.setSize({widthBar, heightBar});
    healthBarBack.setFillColor(sf::Color(100, 0, 0));
    healthBarBack.setPosition({barX, 22.f});
    healthBarBack.setOutlineColor(sf::Color::White);
    healthBarBack.setOutlineThickness(1.f);

    healthBar.setSize({widthBar, heightBar});
    healthBar.setFillColor(sf::Color(200, 0, 0));
    healthBar.setPosition({barX, 22.f});

    shieldBarBack.setSize({widthBar, heightBar});
    shieldBarBack.setFillColor(sf::Color(0, 0, 50));
    shieldBarBack.setPosition({barX, 57.f});
    shieldBarBack.setOutlineColor(sf::Color::Cyan);
    shieldBarBack.setOutlineThickness(1.f);

    shieldBar.setSize({widthBar, heightBar});
    shieldBar.setFillColor(sf::Color::Cyan);
    shieldBar.setPosition({barX, 57.f});

    const sf::Texture &shipTex = resources.getTexture("../assets/img/Ships/enterprise-001.PNG");
    shipLifeIcon.emplace(shipTex);

    float shipScale = 40.f / shipTex.getSize().x;
    shipLifeIcon->setScale({shipScale, shipScale});

    auto setupIcon = [&](string name, string path)
    {
        const sf::Texture &tex = resources.getTexture(path);

        sf::Sprite s(tex);

        float targetSize = 55.f;
        float scaleX = targetSize / tex.getSize().x;
        float scaleY = targetSize / tex.getSize().y;
        s.setScale({scaleX, scaleY});

        powerUpIcons.insert_or_assign(name, s);
    };

    setupIcon("DOUBLE", "../assets/img/PowerUps/PU_Double_Shot.png");
    setupIcon("RAPID", "../assets/img/PowerUps/PU_Rapid_Fire.png");
    setupIcon("INVUL", "../assets/img/PowerUps/PU_Invencibility.png");
}

void HUD::updateStats(int score, int lives, float hp, float maxHp, float shield, float maxShield)
{
    stringstream ss;
    ss << "SCORE: " << setfill('0') << setw(6) << score;
    scoreText.setString(ss.str());

    auto bounds = scoreText.getLocalBounds();
    scoreText.setOrigin({bounds.size.x, 0.f});
    scoreText.setPosition({baseResolution.x - 20.f, 20.f});

    currentLives = lives;

    float hpPercent = (maxHp > 0) ? (hp / maxHp) : 0;
    if (hpPercent < 0)
        hpPercent = 0;
    healthBar.setSize({widthBar * hpPercent, heightBar});

    float shieldPercent = (maxShield > 0) ? (shield / maxShield) : 0;
    if (shieldPercent < 0)
        shieldPercent = 0;
    shieldBar.setSize({widthBar * shieldPercent, heightBar});
}

void HUD::updatePowerUps(bool doubleShot, bool rapidFire, bool invulnerable)
{
    for (auto &pair : powerUpIcons)
    {
        pair.second.setPosition({-100.f, -100.f});
    }

    float startX = 20.f;
    float startY = baseResolution.y - 75.f;
    float spacing = 60.f;
    int index = 0;

    auto placeIcon = [&](string name, bool active)
    {
        if (active && powerUpIcons.count(name))
        {
            powerUpIcons.at(name).setPosition({startX + (index * spacing), startY});
            index++;
        }
    };

    placeIcon("DOUBLE", doubleShot);
    placeIcon("RAPID", rapidFire);
    placeIcon("INVUL", invulnerable);
}

void HUD::draw(sf::RenderWindow &window)
{
    window.draw(scoreText);

    if (healthIcon.has_value())
        window.draw(healthIcon.value());
    if (shieldIcon.has_value())
        window.draw(shieldIcon.value());

    window.draw(healthBarBack);
    window.draw(healthBar);

    window.draw(shieldBarBack);
    window.draw(shieldBar);

    if (shipLifeIcon.has_value())
    {
        float startX = baseResolution.x - 50.f;
        float startY = baseResolution.y - 80.f;
        float spacing = 50.f;

        for (int i = 0; i < currentLives; ++i)
        {
            shipLifeIcon->setPosition({startX - (i * spacing), startY});
            window.draw(shipLifeIcon.value());
        }
    }

    for (auto &pair : powerUpIcons)
    {
        if (pair.second.getPosition().y > 0)
            window.draw(pair.second);
    }
}