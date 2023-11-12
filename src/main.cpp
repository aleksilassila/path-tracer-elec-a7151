/*
* Main file
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/ray.hpp"
#include "world/scene.h"
#include "renderer.h"
#include "pathtracer.hpp"
#include "utils/material.hpp"

void renderLoop(sf::Vector2u &windowSize, Scene &scene) {
    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");

    // Create an image to work with
    sf::Image image;
    image.create(windowSize, sf::Color::Transparent);

    Renderer renderer;
    PathTracer tracer;

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
                                tracer.TestBounceDir(
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

    sf::Vector2u windowSize(900, 900);

    //Camera camera = Camera(Vector(0, 0, 0), Vector(0, 0, 1));
    Camera camera(Vector(0, 0, 0), 0.5, 0.2, 0.2);

    Material matA(sf::Color(240, 120, 40), 0.1, 0);
    Material matB(sf::Color(40, 200, 230), 0.1, 0);
    Material matC(sf::Color::White, 0.1, 1); // EMISSIVE

    Scene scene(camera, {

            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 8), 5, matC)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(4, -4, 4), 2, matB)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(2, 0, 4.2), 1, matA)),
            // std::make_shared<Object::Triangle>(Object::Triangle(Vector(-1, 1, 1),
            //                                 Vector(1, 1, 5), Vector(0 ,0 ,2 ), matB))
    });

    std::cout << scene << std::endl;

    renderLoop(windowSize, scene);
    return 0;
}
