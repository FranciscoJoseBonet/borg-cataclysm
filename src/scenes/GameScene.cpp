#include "GameScene.h"
#include "../core/Game.h"
#include "../effects/Explosion.h"
#include <iostream>
#include <algorithm>

#include "../data/ShipRepository.h"
#include "../core/GameSession.h"

#include "../entities/ships/SpaceShip.h"
#include "../entities/projectiles/LaserProjectile.h"
#include "../entities/projectiles/MissileProjectile.h"

GameScene::GameScene(sf::RenderWindow &w)
    : window(w),
      rng(std::random_device{}()),
      stars(w.getSize(), 400),
      // Inicializamos textos de pausa con el tema global (El LCARS de mi css inventado)
      pauseTitle(UITheme::getInstance().getTitleFont()),
      resumeButton(UITheme::getInstance().getBodyFont()),
      menuButton(UITheme::getInstance().getBodyFont())
{
    // Configuracion de resolucion y camaras
    baseResolution = sf::Vector2f((float)w.getSize().x, (float)w.getSize().y);

    view.setSize(baseResolution);
    view.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    hudView.setSize(baseResolution);
    hudView.setCenter({baseResolution.x / 2.f, baseResolution.y / 2.f});

    // Inicializamos subsistemas
    initPauseMenu();
    hud.init(baseResolution, resources);
    deltaClock.restart();

    // 1. Creamos el player (Desde la seleccion guardada en GameSession)
    int shipId = GameSession::selectedShipIndex;
    const ShipData &shipData = ShipRepository::getShip(shipId);
    const sf::Texture &shipTex = resources.getTexture(shipData.texturePath);

    player = &manager.add<SpaceShip>(shipData, shipTex);
    player->setWorldBounds(baseResolution);
    player->setPosition({baseResolution.x / 2.f, (baseResolution.y / 2.f) + (baseResolution.y / 2.7f)});

    // 2. configuramos los CAllbacks, es la delegacion de responsabilidades
    // Definimos que pasa cuando la nave dispara, muere, etc.

    // A) Callback de Disparo del Jugador
    player->setWeaponsCallback([this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
                               {
        sf::Vector2f direction(0.f, -1.f); // Hacia arriba
        
        if (type == ProjectileType::LASER) 
        {
           const sf::Texture& tex = resources.getTexture(player->getData().primaryWeapon.texturePath);
           
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
           const sf::Texture& missileTex = resources.getTexture(player->getData().secondaryWeapon.texturePath);
           auto& m = manager.add<MissileProjectile>(direction, speed, 1000.f, dmg, missileTex, Faction::Player);
           m.setPosition(pos);
        } });

    // B) Callback de Muerte del Jugador (Explosion visual)
    player->setOnLifeLostCallback([this](sf::Vector2f pos)
                                  {
        const ShipData &data = player->getData();
        const sf::Texture &pExpTex = resources.getTexture(data.explosionTexturePath);

        auto exp = std::make_unique<Explosion>(
            pos, pExpTex, data.explosionFrameSize,
            data.explosionNumFrames, 1.0f
        );
        exp->setScale({0.6f, 0.6f});
        explosions.push_back(std::move(exp)); });

    // 3. Inicializamos el levelManager que se va a encargar del nivel en si
    levelManager = std::make_unique<LevelManager>(manager, resources, baseResolution);

    if (levelManager)
    {
        levelManager->setPlayer(player); // Para que el nivel conozca al jugador (lo usa el boss para targetear el player)

        // Callback para avisar al HUD cuando cambia el nivel y mostrar los cartelitos de lvl
        levelManager->setOnLevelChanged([this](int newLevel)
                                        { hud.showLevelMessage(newLevel); });

        hud.showLevelMessage(1);
    }

    // 4. Los callback de colisiones

    // A) Muerte de Enemigo -> Sumar puntos, explosion y avisar al nivel
    collisionManager.setOnEnemyDeath([this](sf::Vector2f deathPos)
                                     {
                                         score += 100;

                                         const sf::Texture &expTex = resources.getTexture("../assets/img/Ships/SS_Klingon_Destruction.png");
                                         int cols = 2;
                                         int rows = 3;
                                         int frameWidth = expTex.getSize().x / cols;
                                         int frameHeight = expTex.getSize().y / rows;

                                         auto explosion = std::make_unique<Explosion>(
                                             deathPos, expTex, sf::Vector2i(frameWidth, frameHeight),
                                             cols * rows, 0.4f);
                                         explosion->move({0.f, -10.f});
                                         explosion->setRotation(sf::degrees(90.f));
                                         explosion->setScale({0.4f, 0.4f});

                                         explosions.push_back(std::move(explosion));

                                         if (levelManager)
                                             levelManager->notifyEnemyDeath(deathPos); // Posible drop de PowerUp
                                     });

    // B) Impacto de Proyectil -> Chispazo visual (por ahora todos los player usan los mismos)
    collisionManager.setOnProjectileImpact([this](sf::Vector2f impactPos, ProjectileType type)
                                           {
        std::string texturePath;
        int cols = 1; int rows = 1;
        float scale = 1.0f; float duration = 0.2f;

        if (type == ProjectileType::MISSILE) {
            texturePath = "../assets/img/Shots/Missile/SS_Impact_Missile.png"; 
            cols = 8; rows = 1; scale = 1.2f; duration = 0.3f; 
        } else {
            texturePath = "../assets/img/Shots/Laser/SS_Impact_Laser.png";
            cols = 6; rows = 1; scale = 0.6f; duration = 0.2f;
        }

        const sf::Texture& impactTex = resources.getTexture(texturePath);
        int frameWidth = impactTex.getSize().x / cols;
        int frameHeight = impactTex.getSize().y / rows;

        // Usan la clase explosion que sirve para fraccionar el sprotesheet
        auto effect = std::make_unique<Explosion>(
            impactPos, impactTex, sf::Vector2i(frameWidth, frameHeight), 
            cols * rows, duration
        );
        
        effect->move({0.f, -15.f});
        effect->setScale({scale, scale});
        std::uniform_real_distribution<float> rotDist(0.f, 360.f);
        effect->setRotation(sf::degrees(rotDist(rng)));

        explosions.push_back(std::move(effect)); });
}

// Metodos auxiliares

// El menu de pausa
void GameScene::initPauseMenu()
{
    // Fondo semitransparente
    pauseOverlay.setSize(baseResolution);
    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // Titulo
    pauseTitle.setString("SISTEMA PAUSADO");
    pauseTitle.setCharacterSize(60);
    UITheme::applyTitleStyle(pauseTitle);
    auto bounds = pauseTitle.getLocalBounds();
    pauseTitle.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    pauseTitle.setPosition({baseResolution.x / 2.f, baseResolution.y / 3.f});

    // Boton Reanudar
    resumeButton.setString("REANUDAR MISION");
    resumeButton.setCharacterSize(30);
    UITheme::applyMenuOptionStyle(resumeButton, false);
    bounds = resumeButton.getLocalBounds();
    resumeButton.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    resumeButton.setPosition({baseResolution.x / 2.f, baseResolution.y / 2.f});

    // Boton Menu
    menuButton.setString("ABORTAR MISION (MENU)");
    menuButton.setCharacterSize(30);
    UITheme::applyMenuOptionStyle(menuButton, false);
    bounds = menuButton.getLocalBounds();
    menuButton.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    menuButton.setPosition({baseResolution.x / 2.f, (baseResolution.y / 2.f) + 60.f});
}

void GameScene::updateView()
{
    // Mantenemos la lógica de todas las scenes para el texto y elementos centrados si hay cambios de resolucion
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

    // Aplicamos el viewport al juego y al UI
    sf::FloatRect viewport({posX, posY}, {sizeX, sizeY});
    view.setViewport(viewport);
    hudView.setViewport(viewport);
}

// Los metodos del gameloop core

void GameScene::handleEvent(const sf::Event &event)
{
    if (event.is<sf::Event::Resized>())
    {
        updateView();
    }

    // Toggle de Pausa
    if (const auto *keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyEvent->code == sf::Keyboard::Key::Escape)
        {
            isPaused = !isPaused;
            if (!isPaused)
                deltaClock.restart(); // Evita salto de tiempo grande al volver (pausa real)
        }
    }

    // Input en Menu de Pausa
    if (isPaused)
    {
        if (const auto *mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                // Usamos mapPixelToCoords porque la UI tiene su propia View
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
        dt = 0.1f; // Cap para evitar espirales de la muerte por lag

    // 1. Si esta en pausa, solo actualizamos UI de pausa
    if (isPaused)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

        bool resumeHover = resumeButton.getGlobalBounds().contains(mousePos);
        UITheme::applyMenuOptionStyle(resumeButton, resumeHover);

        bool menuHover = menuButton.getGlobalBounds().contains(mousePos);
        UITheme::applyMenuOptionStyle(menuButton, menuHover);
        return;
    }

    // 2. Actualizamos durante el juego
    stars.update(dt);

    if (!gameOver)
    {
        hud.update(dt);
        manager.update(dt);

        if (levelManager)
            levelManager->update(dt); // Spawnea enemigos segun oleadas

        collisionManager.checkCollisions(manager);

        // Actualizar explosiones
        for (auto &exp : explosions)
            exp->update(dt);

        // Borrar explosiones terminadas
        explosions.erase(std::remove_if(explosions.begin(), explosions.end(),
                                        [](const std::unique_ptr<Explosion> &e)
                                        { return e->isFinished(); }),
                         explosions.end());

        // Actualizaciones del HUD
        hud.updateStats(score, player->getLives(), player->getHealth(),
                        player->getData().maxHealth, player->getShield(), player->getMaxShield());
        hud.updatePowerUps(player->isDoubleShotActive(), player->isRapidFireActive(), player->isInvulnerableState());

        // Chequeo del Game Over
        if (player->getLives() <= 0)
        {
            gameOver = true;
        }

        // Limpieza de entidades muertas
        manager.refresh();
    }
}

void GameScene::render()
{
    window.clear(sf::Color::Black);

    // 1. Dibujo de mundo usando la camara del juego
    window.setView(view);
    stars.draw(window);
    manager.draw(window);

    for (const auto &exp : explosions)
    {
        window.draw(*exp);
    }

    // 2. Dibujo de la UI usando la camara fija
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