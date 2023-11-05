/*
* Main file
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/ray.hpp"
#include "world/scene.h"

sf::Color getPixelColor(float x, float y, Scene &scene) {
    // This code calculates if there are >= 1 intersections between a ray
    // shot from cameraOrigin to a direction of (x, y, 1) (where x and y
    // depend on current pixel coordinate and are values from -1 to 1)
    // and a sphere with radius 0.5 and center at (0, 0, 0).

    float xScaled = x * 2 - 1;
    float yScaled = y * 2 - 1;

    Camera camera = scene.GetCamera();
    Ray ray = camera.GetRay(xScaled, yScaled);

    float sphereRadius = 0.5;

    Vector rayDir = ray.GetDirection();
    Vector rayOrig = ray.GetOrigin();

    float a = rayDir * rayDir;
    float b = 2 * (rayDir * rayOrig);
    float c = (rayOrig * rayOrig) - sphereRadius * sphereRadius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) { // No hits
        return sf::Color::Black;
    }

    return sf::Color::Magenta;
}

void renderLoop(sf::Vector2u &windowSize, Scene &scene) {
    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");

    // Create an image to work with
    sf::Image image;
    image.create(windowSize, sf::Color::Transparent);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        for (int x = 0; x < windowSize.x; x++) {
            for (int y = 0; y < windowSize.y; y++) {
                image.setPixel(sf::Vector2u(x, y),
                               getPixelColor((float) x / (float) windowSize.x, (float) y / (float) windowSize.y,
                                             scene));
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

    Camera camera = Camera(Vector(0, 0, -5), Vector(0, 0, 0));

    auto sphere = std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 1), 0.5));
    Scene scene(camera, {sphere});

    std::cout << scene << std::endl;

    renderLoop(windowSize, scene);
    return 0;
}
