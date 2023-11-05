//
// Created by Aleksi on 5.11.2023.
//

#ifndef PATHTRACER_RENDERER_H
#define PATHTRACER_RENDERER_H

#include "SFML/Graphics/Color.hpp"
#include "utils/ray.hpp"
#include "world/scene.h"

class Renderer {
public:
/**
 * Calculates the color of a pixel
 * @param u from -1 to 1
 * @param v from -1 to 1
 * @param scene
 * @return
 */
    sf::Color
    CalculatePixelColor(double u, double v, const Scene &scene) const {
        Camera camera = scene.GetCamera();
        Ray ray = camera.GetRay(u, v);

        double minDistance = std::numeric_limits<double>::max();
        sf::Color color = sf::Color::Black;
        for (const auto &object: scene.GetObjects()) {
            auto intersectionPoint = object->GetIntersectionPoint(ray);
            auto intersectionDistance = intersectionPoint.Distance(ray.GetOrigin());

            if (intersectionDistance < minDistance && intersectionDistance > 0) {
                auto baseColor = object->GetColor();
                auto normal = object->Normal(intersectionPoint);
                auto lightDirection = scene.GetAmbientLightDirection();
                auto lightIntensity = std::max(0.0, normal.Dot(lightDirection));
                color = sf::Color(baseColor.r * lightIntensity,
                                  baseColor.g * lightIntensity,
                                  baseColor.b * lightIntensity);
                minDistance = intersectionDistance;
            }
        }

        return color;
    }
};

#endif //PATHTRACER_RENDERER_H
