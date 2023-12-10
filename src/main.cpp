#include <iostream>
#include <SFML/Graphics.hpp>
#include "world/scene.h"
#include "pathtracer.hpp"
#include <vector>
#include <thread>
#include "utils/filemanager.hpp"

/**
 * The main file
 */


const int threadCount = std::max(1, (int) (std::thread::hardware_concurrency() * 0.8));

/**
 * Loop that handles rendering
 * @param windowSize
 * @param scene
 */
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
                    FileManager::SaveRenderImage("out.png", image);
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
                    } else if (event.key.code == sf::Keyboard::Y) {
                        camera.MoveUp();
                        resetContext = true;
                    } else if (event.key.code == sf::Keyboard::H) {
                        camera.MoveDown();
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

/**
 * Main function that reads from file and starts the rendering
 */
int main() {
    sf::Vector2u windowSize(400, 400);

    std::string filePath = "/files/scene1.json";
    Scene scene = FileManager::CreateScene(filePath);
    std::cout << scene << std::endl;
    std::cout << "Number of threads used: " << threadCount << std::endl;

    renderLoop(windowSize, scene);
    return 0;
}
