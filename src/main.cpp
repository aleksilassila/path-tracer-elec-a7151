/*
* Main file
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/ray.hpp"
#include "world/scene.h"
#include "renderer.h"
#include "utils/material.hpp"

void renderLoop(sf::Vector2u &windowSize, Scene &scene) {
    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");

    // Create an image to work with
    sf::Image image;
    image.create(windowSize, sf::Color::Transparent);

    Renderer renderer;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        for (unsigned int x = 0; x < windowSize.x; x++) {
            for (unsigned int y = 0; y < windowSize.y; y++) {
                double scaledX = (double) x * 2 / (double) windowSize.x - 1;
                double scaledY = (double) y * 2 / (double) windowSize.y - 1;
                image.setPixel(sf::Vector2u(x, y),
                               renderer.CalculatePixelColor(
                                       scaledX,
                                       scaledY,
                                       scene
                               ));
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
    }
}

int main() {
    sf::Vector2u windowSize(600, 600);

    Camera camera = Camera(Vector(0, 0, 0), Vector(0, 0, 1));

    Material matA(sf::Color::Red, 0.0);
    Material matB(sf::Color::Green, 0.5);

    Scene scene(camera, {

            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 8), 5, matA)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(-0.2, 0, 1), 0.5)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(2, 0, 4.2), 1, matB))
    });

    std::cout << scene << std::endl;

    renderLoop(windowSize, scene);
    return 0;
}
