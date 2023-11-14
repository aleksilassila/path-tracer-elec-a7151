/*
* Main file
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/ray.hpp"
#include "world/scene.h"
#include "utils/material.hpp"
#include "pathtracer.hpp"

void renderLoop(sf::Vector2u &windowSize, Scene &scene) {
    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");

    // Create an image to work with
    sf::Image image;
    image.create(windowSize, sf::Color::Transparent);

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
                double scaledY = -((double) y * 2 / (double) windowSize.y) + 1;
                image.setPixel(sf::Vector2u(x, y),
                               tracer.GetPixelColor(
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
    sf::Vector2u windowSize(400, 400);

    //Camera camera = Camera(Vector(0, 0, 0), Vector(0, 0, 1));
    Camera camera(Vector(0, 0, 0), 6, 0, 0.0);
    camera.LookAt(Vector(-4, 4, 70)); // Look at the mirror
    camera.SetFovDeg(30);

    Material matA(sf::Color(200, 100, 40), 0.4);
    Material matB(sf::Color(100, 180, 150), 0.5);
    Material matC(sf::Color(30, 30, 150), 0.9);
    Material matD(sf::Color(200, 60, 200), 0.6);
    Material mirror(sf::Color::White, 0.03);

    Material light(sf::Color::Black, 0.5, 1.0);

    Scene scene(camera, {

            std::make_shared<Object::Sphere>(Object::Sphere(Vector(-4, 4, 70), 4, mirror)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(10, 0, 48), 7, matA)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, -5, 36), 2, matB)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(-2, 4, 41), 2, matB)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(4, 1, 40), 2, matC)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(3, 3, 39), 2, matD)),

            // Room
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, -2005, 0), 2000, matA)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 2005, 0), 2000, matB)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(-2005, 0, 0), 2000, light)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(2005, 0, 0), 2000, matC)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, -1005), 2000, matD)),

    });

    std::cout << scene << std::endl;

    renderLoop(windowSize, scene);
    return 0;
}
