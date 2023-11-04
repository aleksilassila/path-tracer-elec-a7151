/*
* Main file
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/ray.hpp"

sf::Color getPixelColor(float x, float y) {
    // This code calculates if there are >= 1 intersections between a ray
    // shot from cameraOrigin to a direction of (x, y, 1) (where x and y
    // depend on current pixel coordinate and are values from -1 to 1)
    // and a sphere with radius 0.5 and center at (0, 0, 0).

    Vector cameraOrigin = Vector(0, 0, -1);
    float sphereRadius = 0.5;

    float xScaled = x * 2 - 1;
    float yScaled = y * 2 - 1;

    Ray ray = Ray(cameraOrigin, Vector(xScaled, yScaled, 1).Norm());
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

int main() {
    unsigned int width = 600;
    unsigned int height = 600;

    sf::Vector2u windowSize(width, height);

    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");

    // Create an image to work with
    sf::Image image;
    image.create(windowSize, sf::Color::Transparent);

    int index = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Modify specific pixels each frame
//        for (int x = 100; x < 100 + index; x++) {
//            for (int y = 100; y < 200; y++) {
//                image.setPixel(sf::Vector2u(x, y), sf::Color::Red);
//            }
//        }

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                image.setPixel(sf::Vector2u(x, y), getPixelColor((float) x / width, (float) y / height));
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
