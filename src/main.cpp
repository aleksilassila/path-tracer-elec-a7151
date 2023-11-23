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
#include <thread>
#include "utils/filemanager.hpp"

const int threadCount = std::max(1, (int) (std::thread::hardware_concurrency() * 0.8));

//void
//renderIndexes(int startIndex, int endIndex, int frameCount, std::vector<sf::Vector3f> &colorBuffer,
//              sf::Vector2u &windowSize,
//              Scene &scene, sf::Image &image) {
//    PathTracer tracer;
//
//    double aspectRatio = (double) windowSize.x / (double) windowSize.y;
//    for (unsigned int x = startIndex; x < endIndex; x++) {
//        for (unsigned int y = 0; y < windowSize.y; y++) {
//            double scaledX = ((double) x * 2 / (double) windowSize.x - 1) * std::min(1.0, aspectRatio);
//            double scaledY = (-((double) y * 2 / (double) windowSize.y) + 1) / std::max(1.0, aspectRatio);
//
//            sf::Color pixelColor = tracer.GetPixelColor(scaledX, scaledY, scene, x * y * frameCount);
//            sf::Vector3f currentColor(pixelColor.r, pixelColor.g, pixelColor.b);
//
//            // Accumulate color
//            unsigned int bufferIndex = y * windowSize.x + x;
//            colorBuffer[bufferIndex] += currentColor;
//
//            // average color over the frames
//            sf::Vector3f averagedColor = colorBuffer[bufferIndex] / static_cast<float>(frameCount + 1);
//
//            image.setPixel(sf::Vector2u(x, y), sf::Color(averagedColor.x, averagedColor.y, averagedColor.z));
//        }
//    }
//}

void renderLoop(sf::Vector2u &windowSize, Scene &scene) {
    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");

    PathTracer tracer(windowSize, scene);

    // Start render thread
    std::thread renderer([&tracer, &window]() {
        tracer.Renderer(window);
    });

    // Update loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            bool resetContext = false;

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                windowSize = window.getSize();
//                image.create(windowSize, sf::Color::Transparent);
                sf::FloatRect view(sf::Vector2f(0, 0), sf::Vector2f(event.size.width, event.size.height));
                window.setView(sf::View(view));

                resetContext = true;
                // Reset color buffer
//                colorBuffer = std::vector<sf::Vector3f>(windowSize.x * windowSize.y, sf::Vector3f(0, 0, 0));
//                frameCount = 0;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    sf::Image image = tracer.GetLatestImage();
                    FileManager::saveRenderImage("out.png", image);
                } else {
                    Camera &camera = scene.GetCamera();

                    if (event.key.code == sf::Keyboard::Left) {
                        camera.LookRight();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        camera.LookLeft();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::Up) {
                        camera.LookUp();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::Down) {
                        camera.LookDown();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::W) {
                        camera.MoveForward();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::S) {
                        camera.MoveBackward();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::A) {
                        camera.MoveLeft();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::D) {
                        camera.MoveRight();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::Q) {
                        camera.MoveUpAlongYaxis();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::E) {
                        camera.MoveDownAlongYaxis();
                        resetContext = true;

                        // Set moving speed and angle change amount
                    } else if (event.key.code == sf::Keyboard::U) {
                        camera.IncrementLookSensitivity();
                    } else if (event.key.code == sf::Keyboard::J) {
                        camera.DecrementLookSensitivity();
                    } else if (event.key.code == sf::Keyboard::O) {
                        camera.IncrementMoveSpeed();
                    } else if (event.key.code == sf::Keyboard::L) {
                        camera.DecrementMoveSpeed();
                    }
                }
            }

            // If the camera has moved, send the new render context to the renderer thread
            if (resetContext) {
                tracer.UpdateRenderContext(windowSize, scene);
            }
        }

        sf::Image image = tracer.GetLatestImage();
        if (image.getSize().x == 0 || image.getSize().y == 0) {
            continue;
        }

        sf::Texture texture;
        bool textureDidLoad = texture.loadFromImage(image);
        if (!textureDidLoad) continue;

        sf::Sprite sprite(texture);
//        sprite.setScale(sf::Vector2f(4, 4));

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

int main() {
    sf::Vector2u windowSize(400, 400);

    //Camera camera = Camera(Vector(0, 0, 0), Vector(0, 0, 1));
    Camera camera(Vector(0, 0, 0), 6, 0.0, 0.0);
    camera.LookAt(Vector(0, 0, 70)); // Look at the mirror
    camera.SetFovDeg(80);

    // Diffuse materials
    Material matA(sf::Color(200, 100, 40), 0.4, 0.4, 0);
    Material matC(sf::Color(50, 100, 250), 0.7, 0, 0);
    Material matD(sf::Color(200, 60, 200), 0.6, 0, 0);
    Material matF(sf::Color(200, 200, 200), 1.0, 0, 0);
    Material matG(sf::Color(144, 20, 10), 0.75, 0, 0);

    // Specular material
    Material matE(sf::Color(250, 250, 10), 0.16, 1.0, 0, sf::Color(250, 250, 10), Vector(0, 0, 0));
    Material mirror(sf::Color(245, 245, 245), 0.008, 1.0, 0);
    Material ceramic(sf::Color(240, 240, 240), 1, 0.3, 0.005, sf::Color::White);

    // Transparent material:
    Material matB(sf::Color(100, 180, 150), 0.75, 0, 0);

    // Emissive materials
    Material lightA(sf::Color::Black, 0.5, 0.5, 0, sf::Color::Magenta, Vector(1, 1, 1));
    Material lightB(sf::Color::Black, 0.5, 0.5, 0, sf::Color::White, Vector(0.2, 0.5, 1));
    Material lightC(sf::Color::Black, 0.5, 0.5, 0, sf::Color::White, Vector(1, 0.8, 0.6));
    Material lightD(sf::Color::Black, 0.5, 0.5, 0, sf::Color::White, Vector(0.75, 1, 0.75));


    Scene scene(camera, {
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 44), 3, lightA)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(4, -3, 48), 3, matA)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 44), 3, mirror)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(4, -3, 48), 3, matA)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, -5, 36), 2, matB)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(-2, 4, 41), 2, matC)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(-3, -2, 40), 1.2, matD)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(-5, -1, 36), 2, ceramic)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(3, 3, 39), 1.8, matE)),
//
//            // Room
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, -2005, 0), 2000, matF)), // floor
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 2010, 0), 2000, lightA)), // roof
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 0), 2000, lightA)), // roof
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(-2010, 0, 0), 2000, mirror)), // side
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(2010, 0, 0), 2000, mirror)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, 2100), 2000, matC)),
            std::make_shared<Object::Sphere>(Object::Sphere(Vector(0, 0, -2000), 2000, matA)),
    });

    std::cout << scene << std::endl;
    std::cout << "Number of threads used: " << threadCount << std::endl;

    renderLoop(windowSize, scene);
    return 0;
}
