/*
* Main file
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/ray.hpp"
#include "world/scene.h"
#include "utils/material.hpp"
#include "pathtracer.hpp"
#include <vector>
#include "utils/filemanager.hpp"

void renderLoop(sf::Vector2u &windowSize, Scene &scene) {
    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");

    // Create an image to work with
    sf::Image image;
    image.create(windowSize, sf::Color::Transparent);

    std::vector<sf::Vector3f> colorBuffer(windowSize.x * windowSize.y, sf::Vector3f(0, 0, 0));

    PathTracer tracer;

    // Filemanager to save images.
    FileManager filemanager("out.png");
    unsigned int maxFrameCount = 32;
    unsigned int frameCount = 0;

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

                sf::Color pixelColor = tracer.GetPixelColor(scaledX, scaledY, scene);
                sf::Vector3f currentColor(pixelColor.r, pixelColor.g, pixelColor.b);

                // Accumulate color
                unsigned int bufferIndex = y * windowSize.x + x;
                colorBuffer[bufferIndex] += currentColor;

                // average color over the frames
                sf::Vector3f averagedColor = colorBuffer[bufferIndex] / static_cast<float>(frameCount + 1);

                image.setPixel(sf::Vector2u(x,y), sf::Color(averagedColor.x, averagedColor.y, averagedColor.z));
            }
        }

        if (frameCount >= maxFrameCount) {
            // once enough frames have been accumulated save image and quit program
            filemanager.saveRenderImage(image);
            return;
        }

        // Load the image into a texture and display it
        sf::Texture texture;
        bool didLoad = texture.loadFromImage(image);
        sf::Sprite sprite(texture);

        window.clear();

        // Draw the sprite with the averaged colored pixels
        window.draw(sprite);

        window.display();

        // Update frame count
        frameCount++;

    }
}

int main() {
    sf::Vector2u windowSize(1024, 1024);

    //Camera camera = Camera(Vector(0, 0, 0), Vector(0, 0, 1));
    Camera camera(Vector(0, 0, 0), 6, 0, 0.0);
    camera.LookAt(Vector(-4, 4, 70)); // Look at the mirror
    camera.SetFovDeg(30);

    // Diffuse materials
    Material matA(sf::Color(200, 100, 40), 0.4);
    Material matB(sf::Color(100, 180, 150), 0.5);
    Material matC(sf::Color(50, 100, 250), 0.9);
    Material matD(sf::Color(200, 60, 200), 0.6);
    Material matE(sf::Color(250,250,10), 0.16);
    Material matF(sf::Color(200,200,200), 1.0);
    Material matG(sf::Color(144,20,10), 0.75);
    
    Material mirror(sf::Color(255,255,255), 0.008);

    // Emissive materials
    Material lightA(sf::Color::Black, 0.5, Vector(1,1,1));
    Material lightB(sf::Color::Black, 0.5, Vector(0.2, 0.5, 1));
    Material lightC(sf::Color::Black, 0.5, Vector(1, 0.8, 0.6));
    Material lightD(sf::Color::Black, 0.5, Vector(0.75, 1, 0.75));


    Scene scene(camera, {

        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 44), 3, mirror)),
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(4, -3, 48), 3, matA)),
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, -5, 36), 2, matB)),
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(-2, 4, 41), 2, matC)),
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(-3, -2, 40), 1.2, matD)),
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(-5, -1, 36), 2, matA)),
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(3, 3, 39), 1.8, matE)),

        // Room 
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, -2010, 0), 2000,  matA)), // side 
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 2010, 0), 2000, matB)), // side
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(-2010, 0, 0), 2000, lightA)), // roof
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(2005, 0, 0), 2000, matF)),
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 2100), 2000, matC)), 
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, -2000), 2000, lightC)), 

    });

    std::cout << scene << std::endl;

    renderLoop(windowSize, scene);
    return 0;
}
