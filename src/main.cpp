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

#define WINDOW_TARGET_FPS 60.0


const int threadCount = std::max(1, (int) (std::thread::hardware_concurrency() * 0.8));

/**
 * Loop that handles rendering
 * @param windowSize
 * @param scene
 */
void renderLoop(sf::Vector2u &windowSize, Scene &scene) {
    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");
    window.setFramerateLimit(WINDOW_TARGET_FPS);

    PathTracer tracer(windowSize, scene);

    window.setActive(false);
    // Start render thread
    std::thread renderer([&tracer, &window]() {
        tracer.Renderer(window);
    });

    auto lastMousePosition = sf::Vector2u(0, 0);
    // Update loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            bool resetContext = false;
            bool renderPreviewFrame = false;

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                windowSize = window.getSize();
//                image.create(windowSize, sf::Color::Transparent);
                sf::FloatRect view(sf::Vector2f(0, 0), sf::Vector2f(event.size.width, event.size.height));
                window.setView(sf::View(view));

                resetContext = true;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    lastMousePosition = sf::Vector2u(event.mouseButton.x, event.mouseButton.y);
//
//                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    renderPreviewFrame = false;
                    resetContext = true;
                }
            } else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2u currentMousePosition = sf::Vector2u(event.mouseMove.x, event.mouseMove.y);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2u screenResolution = sf::VideoMode::getDesktopMode().size;

                    double deltaX =
                            (double) ((int) currentMousePosition.x - (int) lastMousePosition.x) /
                            (double) screenResolution.x;
                    double deltaY =
                            (double) ((int) currentMousePosition.y - (int) lastMousePosition.y) /
                            (double) screenResolution.y;

                    Camera &camera = scene.GetCamera();
                    camera.Rotate(deltaX * 2 * M_PI, deltaY * M_PI);
                    resetContext = true;
                    renderPreviewFrame = true;
                }
                lastMousePosition = currentMousePosition;
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
                tracer.UpdateRenderContext(windowSize, scene, renderPreviewFrame);
            }
        }
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
