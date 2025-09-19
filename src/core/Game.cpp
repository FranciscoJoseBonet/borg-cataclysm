#include "Game.h"

Game::Game()
    : window(sf::VideoMode({800, 600}), "Borg Cataclysm")
{
    window.setFramerateLimit(60);
}

void Game::run()
{
    sf::RectangleShape rect(sf::Vector2f(100.f, 100.f));
    rect.setFillColor(sf::Color::Red);
    rect.setPosition(sf::Vector2f(350.f, 250.f));

    const float speed = 200.f; // pixeles por segundo
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time dt = clock.restart();

        // Manejo de eventos
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Movimiento con flechas
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            movement.x -= speed * dt.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            movement.x += speed * dt.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            movement.y -= speed * dt.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            movement.y += speed * dt.asSeconds();

        rect.move(movement);

        // Dibujar
        window.clear(sf::Color::Black);
        window.draw(rect);
        window.display();
    }
}
