#include <SFML/Window.hpp>

int Game()
{

    sf::Window window(sf::VideoMode({800, 600}), "Borg Cataclysm");

    // Bucle principal de ciclo de vida de la ventana
    while (window.isOpen())
    {
        // Bucle de manejo de la cola de eventos en la ventana
        while (const std::optional event = window.pollEvent())
        {
            // Se cierra si se hace click en close
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}