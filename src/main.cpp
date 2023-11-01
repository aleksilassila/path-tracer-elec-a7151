/*
* Main file
*/

#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600), 32), "SFML Window");

    // Create an image to work with
    sf::Image image;
    image.create(sf::Vector2u(800, 600), sf::Color::Transparent);

    int index = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Modify specific pixels each frame
        for (int x = 100; x < 100 + index; x++) {
            for (int y = 100; y < 200; y++) {
                image.setPixel(sf::Vector2u(x, y), sf::Color::Red);
            }
        }

        // Load the image into a texture and display it
        sf::Texture texture;
        bool didLoad = texture.loadFromImage(image);
        sf::Sprite sprite(texture);

        window.clear();

        // Draw the sprite with the colored pixels
        window.draw(sprite);

        window.display();
        index++;
    }

    return 0;
}