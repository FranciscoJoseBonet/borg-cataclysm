#include "GameScene.h"
#include "../core/Game.h"
#include <iostream>

#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Scout.h"

#include "../entities/projectiles/LaserProjectile.h"
#include "../entities/projectiles/MissileProjectile.h"
#include "../entities/items/PowerUp.h"

GameScene::GameScene(sf::RenderWindow &w)
    : window(w),
      rng(std::random_device{}()),
      stars(w.getSize(), 400),
      pauseTitle(font),
      resumeButton(font),
      menuButton(font)
{
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);

    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    hudView.setSize(baseResolution);
    hudView.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    initPauseMenu();
    hud.init(baseResolution, resources);

    deltaClock.restart();
    player = &manager.add<SpaceShip>();

    player->setWorldBounds(baseResolution);

    player->setWeaponsCallback([this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
                               {
        sf::Vector2f direction(0.f, -1.f);
        if (type == ProjectileType::LASER) 
        {
           const sf::Texture& tex = resources.getTexture("../assets/img/Federation_Shot_1.png");
           
           if (player->isDoubleShotActive()) {
               auto& p1 = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Player);
               p1.setPosition({pos.x - 25.f, pos.y}); 
               auto& p2 = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Player);
               p2.setPosition({pos.x + 25.f, pos.y});
           } else {
               auto& p = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Player);
               p.setPosition(pos);
           }
        }
        else if (type == ProjectileType::MISSILE) 
        {
            const sf::Texture& missileTex = resources.getTexture("../assets/img/Federation_Shot_2.png");
            auto& m = manager.add<MissileProjectile>(direction, speed, 1000.f, dmg, missileTex, Faction::Player);
            m.setPosition(pos);
        } });

    player->setPosition({baseResolution.x / 2.f, (baseResolution.y / 2.f) + (baseResolution.y / 2.7f)});

    collisionManager.setOnEnemyDeath([this](sf::Vector2f deathPos)
                                     {
        score += 100;
        std::uniform_real_distribution<float> chanceDist(0.f, 1.f);
        if (chanceDist(rng) <= 0.4f) spawnPowerUp(deathPos); });

    spawnEnemyWave(15);
}

void GameScene::updateView()
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
    hudView.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
}

void GameScene::initPauseMenu()
{
    if (!font.openFromFile("../assets/fonts/Star_Trek_Enterprise_Future.ttf"))
    {
        if (!font.openFromFile("../assets/fonts/pixel_font.ttf"))
            std::cerr << "ERROR: No se pudo cargar fuente para pausa.\n";
    }

    pauseOverlay.setSize(baseResolution);
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    pauseTitle.setFont(font);
    pauseTitle.setString("SISTEMA PAUSADO");
    pauseTitle.setCharacterSize(60);
    pauseTitle.setFillColor(sf::Color::Cyan);

    auto bounds = pauseTitle.getLocalBounds();
    pauseTitle.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    pauseTitle.setPosition({baseResolution.x / 2.f, baseResolution.y / 3.f});

    resumeButton.setFont(font);
    resumeButton.setString("REANUDAR MISION");
    resumeButton.setCharacterSize(30);
    resumeButton.setFillColor(sf::Color::White);

    bounds = resumeButton.getLocalBounds();
    resumeButton.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    resumeButton.setPosition({baseResolution.x / 2.f, baseResolution.y / 2.f});

    menuButton.setFont(font);
    menuButton.setString("ABORTAR MISION (MENU)");
    menuButton.setCharacterSize(30);
    menuButton.setFillColor(sf::Color::White);

    bounds = menuButton.getLocalBounds();
    menuButton.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    menuButton.setPosition({baseResolution.x / 2.f, (baseResolution.y / 2.f) + 60.f});
}

void GameScene::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Resized>())
    {
        updateView();
    }

    if (const auto *keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyEvent->code == sf::Keyboard::Key::Escape)
        {
            isPaused = !isPaused;
            if (!isPaused)
                deltaClock.restart();
        }
    }

    if (isPaused)
    {
        if (const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

                if (resumeButton.getGlobalBounds().contains(mousePos))
                {
                    isPaused = false;
                    deltaClock.restart();
                }
                else if (menuButton.getGlobalBounds().contains(mousePos))
                {
                    nextScene = SceneType::Menu;
                }
            }
        }
    }
}

void GameScene::update()
{
    float dt = deltaClock.restart().asSeconds();
    if (dt > 0.1f)
        dt = 0.1f;

    if (isPaused)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

        if (resumeButton.getGlobalBounds().contains(mousePos))
            resumeButton.setFillColor(sf::Color::Yellow);
        else
            resumeButton.setFillColor(sf::Color::White);

        if (menuButton.getGlobalBounds().contains(mousePos))
            menuButton.setFillColor(sf::Color::Red);
        else
            menuButton.setFillColor(sf::Color::White);

        return;
    }

    stars.update(dt);

    if (!gameOver)
    {
        manager.update(dt);
        collisionManager.checkCollisions(manager);

        hud.updateStats(score, player->getLives(), player->getHealth(), 100.f, player->getShield(), player->getMaxShield());
        hud.updatePowerUps(player->isDoubleShotActive(), player->isRapidFireActive(), player->isInvulnerableState());

        if (player->getLives() <= 0)
        {
            gameOver = true;
        }
        manager.refresh();
    }
}

void GameScene::render()
{
    window.clear(sf::Color::Black);

    window.setView(view);
    stars.draw(window);
    manager.draw(window);

    window.setView(hudView);
    hud.draw(window);

    if (isPaused)
    {
        window.draw(pauseOverlay);
        window.draw(pauseTitle);
        window.draw(resumeButton);
        window.draw(menuButton);
    }

    window.display();
}

void GameScene::spawnEnemyWave(int count)
{
    std::uniform_real_distribution<float> distX(50.f, baseResolution.x - 50.f);
    std::uniform_real_distribution<float> distY(-2000.f, -100.f);
    std::uniform_real_distribution<float> distFireRate(0.5f, 1.5f);
    std::uniform_real_distribution<float> distShipSpeed(80.f, 150.f);
    std::uniform_real_distribution<float> distProjSpeed(300.f, 600.f);

    const sf::Texture &scoutTex = resources.getTexture("../assets/img/Klingon_Ship_1.png");

    auto enemyFireAction = [this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
    {
        sf::Vector2f direction(0.f, 1.f);
        const sf::Texture &tex = resources.getTexture("../assets/img/Klingon_Shot_1.png");

        auto &p = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Alien);
        p.setPosition(pos);
    };

    for (int i = 0; i < count; ++i)
    {
        float x = distX(rng);
        float y = distY(rng);

        auto &enemy = manager.add<Scout>(scoutTex, sf::Vector2f(x, y));

        enemy.setSpeed(distShipSpeed(rng));
        enemy.setFireRate(distFireRate(rng));
        enemy.setProjectileSpeed(distProjSpeed(rng));

        enemy.setFireCallback(enemyFireAction);
        enemy.setHealth(30.f);
    }
}

void GameScene::spawnPowerUp(sf::Vector2f position)
{
    std::uniform_int_distribution<int> distChance(0, 105);
    int roll = distChance(rng);

    PowerUpType type;

    if (roll <= 35)
        type = PowerUpType::SHIELD;
    else if (roll <= 65)
        type = PowerUpType::RAPID_FIRE;
    else if (roll <= 85)
        type = PowerUpType::DOUBLE_SHOT;
    else if (roll <= 94)
        type = PowerUpType::INVINCIBILITY;
    else if (roll <= 100)
        type = PowerUpType::HEAL;
    else
        type = PowerUpType::EXTRA_LIFE;

    std::string texturePath;
    switch (type)
    {
    case PowerUpType::SHIELD:
        texturePath = "../assets/img/PU_Extra_Shield.png";
        break;
    case PowerUpType::DOUBLE_SHOT:
        texturePath = "../assets/img/PU_Double_Shot.png";
        break;
    case PowerUpType::RAPID_FIRE:
        texturePath = "../assets/img/PU_Rapid_Fire.png";
        break;
    case PowerUpType::INVINCIBILITY:
        texturePath = "../assets/img/PU_Invencibility.png";
        break;
    case PowerUpType::HEAL:
        texturePath = "../assets/img/PU_Heal.png";
        break;
    case PowerUpType::EXTRA_LIFE:
        texturePath = "../assets/img/PU_Extra_Life_Rainbow.png";
        break;
    }

    const sf::Texture &tex = resources.getTexture(texturePath);
    manager.add<PowerUp>(position, type, tex);
}