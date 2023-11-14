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
    unsigned long long frameCount = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                windowSize = window.getSize();
                image.create(windowSize, sf::Color::Transparent);
                sf::FloatRect view(sf::Vector2f(0, 0), sf::Vector2f(event.size.width, event.size.height));
                window.setView(sf::View(view));

                // Reset color buffer
                colorBuffer = std::vector<sf::Vector3f>(windowSize.x * windowSize.y, sf::Vector3f(0, 0, 0));
                frameCount = 0;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    filemanager.saveRenderImage(image);
                } else {
                    Camera &camera = scene.GetCamera();
                    double pitch = camera.GetPitch();
                    double yaw = camera.GetYaw();

                    if (event.key.code == sf::Keyboard::Left) {
                        camera.Rotate(0.1, 0);
                    } else if (event.key.code == sf::Keyboard::Right) {
                        camera.Rotate(-0.1, 0);
                    } else if (event.key.code == sf::Keyboard::Up) {
                        camera.Rotate(0, 0.1);
                    } else if (event.key.code == sf::Keyboard::Down) {
                        camera.Rotate(0, -0.1);
                    } else if (event.key.code == sf::Keyboard::W) {
                        camera.Move(std::sin(yaw) * std::cos(pitch), std::sin(pitch), std::cos(yaw) * std::cos(pitch));
                    } else if (event.key.code == sf::Keyboard::S) {
                        camera.Move(-std::sin(yaw) * std::cos(pitch), -std::sin(pitch),
                                    -std::cos(yaw) * std::cos(pitch));
                    } else if (event.key.code == sf::Keyboard::A) {
                        camera.Move(std::cos(yaw), 0, -std::sin(yaw));
                    } else if (event.key.code == sf::Keyboard::D) {
                        camera.Move(-std::cos(yaw), 0, std::sin(yaw));
                    } else if (event.key.code == sf::Keyboard::Q) {
                        camera.Move(0, 1, 0);
                    } else if (event.key.code == sf::Keyboard::E) {
                        camera.Move(0, -1, 0);
                    }

                    colorBuffer = std::vector<sf::Vector3f>(windowSize.x * windowSize.y, sf::Vector3f(0, 0, 0));
                    frameCount = 0;
                }
            }
        }

        double aspectRatio = (double) windowSize.x / (double) windowSize.y;
        for (unsigned int x = 0; x < windowSize.x; x++) {
            for (unsigned int y = 0; y < windowSize.y; y++) {
                double scaledX = ((double) x * 2 / (double) windowSize.x - 1) * std::min(1.0, aspectRatio);
                double scaledY = (-((double) y * 2 / (double) windowSize.y) + 1) / std::max(1.0, aspectRatio);

                /* An implementation without the frame buffer
                sf::Vector2u pixel(x, y);
                sf::Color previousColor = image.getPixel(pixel);
                sf::Color nextColor = tracer.GetPixelColor(scaledX, scaledY, scene);;

                if (frameCount == 0) {
                    image.setPixel(pixel, nextColor);
                } else {
                    image.setPixel(pixel,
                                   sf::Color((previousColor.r * (frameCount) + nextColor.r) / (frameCount + 1),
                                             (previousColor.g * (frameCount) + nextColor.g) / (frameCount + 1),
                                             (previousColor.b * (frameCount) + nextColor.b) / (frameCount + 1)));
                }
                */

                sf::Color pixelColor = tracer.GetPixelColor(scaledX, scaledY, scene);
                sf::Vector3f currentColor(pixelColor.r, pixelColor.g, pixelColor.b);

                // Accumulate color
                unsigned int bufferIndex = y * windowSize.x + x;
                colorBuffer[bufferIndex] += currentColor;

                // average color over the frames
                sf::Vector3f averagedColor = colorBuffer[bufferIndex] / static_cast<float>(frameCount + 1);

                image.setPixel(sf::Vector2u(x, y), sf::Color(averagedColor.x, averagedColor.y, averagedColor.z));
            }
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
    sf::Vector2u windowSize(256, 256);

    //Camera camera = Camera(Vector(0, 0, 0), Vector(0, 0, 1));
    Camera camera(Vector(0, 0, 0), 6, 0.0, 0.0);
    camera.LookAt(Vector(0, 0, 70)); // Look at the mirror
    camera.SetFovDeg(30);

    // Diffuse materials
    Material matA(sf::Color(200, 100, 40), 0.4);
    Material matB(sf::Color(100, 180, 150), 0.5);
    Material matC(sf::Color(50, 100, 250), 0.9);
    Material matD(sf::Color(200, 60, 200), 0.6);
    Material matE(sf::Color(250, 250, 10), 0.16);
    Material matF(sf::Color(200, 200, 200), 1.0);
    Material matG(sf::Color(144, 20, 10), 0.75);

    Material mirror(sf::Color(255, 255, 255), 0.008);

    // Emissive materials
    Material lightA(sf::Color::Black, 0.5, Vector(1, 1, 1));
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
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, -2005, 0), 2000,  matF)), // floor
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 2010, 0), 2000, lightA)), // roof
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(-2010, 0, 0), 2000, matB)), // side
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(2010, 0, 0), 2000, matG)),
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 2100), 2000, matC)), 
        std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, -2000), 2000, lightC)), 
    });

    std::cout << scene << std::endl;

    renderLoop(windowSize, scene);
    return 0;
}
