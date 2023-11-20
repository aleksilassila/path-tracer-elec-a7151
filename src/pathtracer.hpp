#ifndef PATHTRACER_HPP
#define PATHTRACER_HPP

#include <mutex>
#include <atomic>
#include <thread>
#include "SFML/Graphics/Color.hpp"
#include "utils/ray.hpp"
#include "world/scene.h"
#include "utils/rand.h"

struct HitInfo {

    // Did ray hit an object
    bool hit;
    //point hit
    Vector point;
    // Surface normal of point hit
    Vector sNormal;
    // Material of object hit
    Material sMaterial;

    HitInfo() : hit(false), point(), sNormal(), sMaterial() {}
};

struct RenderContext {
    const Scene scene;
    const sf::Vector2u dimensions;

//    const std::vector<sf::Vector3f> colorBuffer;
    sf::Image image;
    int frameCount;

    std::mutex mutex;

    RenderContext(sf::Vector2u &dimensions, Scene &scene) : scene(scene), dimensions(dimensions),
                                                            frameCount(0) {
        image.create(dimensions, sf::Color::Transparent);
    }
};

class PathTracer {

private:

    const int threadCount = std::max(1, (int) (std::thread::hardware_concurrency() * 0.8));
    int maxBounces_;
    std::shared_ptr<RenderContext> context_;
    std::mutex contextMutex_;

    sf::Image image_;
    std::mutex imageMutex_;

public:
    PathTracer(sf::Vector2u &dimensions, Scene &scene) : PathTracer(dimensions, scene, 12) {}

    PathTracer(sf::Vector2u &dimensions, Scene &scene, int maxBounces) : maxBounces_(maxBounces), context_(
            std::make_shared<RenderContext>(dimensions, scene)
    ) {}


    ~PathTracer();

    /**
     * @brief Get the Pixel Color
     * 
     * @param u 
     * @param v 
     * @param scene 
     * @return sf::Color 
     */
    static sf::Color GetPixelColor(double u, double v, Scene &scene, unsigned int randSeed, int maxBounces) {
        // Initialize color components as white
        double R = 1, G = 1, B = 1;
        // No light in the beginning
        Vector light(0, 0, 0);

        Camera camera = scene.GetCamera();

        //Start with camera ray.
        Ray ray_ = camera.CastRay(u, v);

        for (int b = 0; b < maxBounces; b++) {
            HitInfo lastHit_ = GetNearestHitInfo(ray_, scene);

            if (lastHit_.hit) {
                if (lastHit_.sMaterial.getEmission().Len() > 0.0) {
                    // Break out of loop if ray hits a light.
                    light += lastHit_.sMaterial.getEmission();
                    break;
                }

                Vector point = lastHit_.point;
                Vector normal = lastHit_.sNormal;
                Material material = lastHit_.sMaterial;
                Vector newRayDir;
                sf::Color surfaceCol;

                // Handle diffuse and specular reflections
                double specularChance = Random::GetRandomDoubleUniform(0.0, 1.0,
                                                                       static_cast<unsigned int> (u * v * randSeed *
                                                                                                  456789) *
                                                                       std::numeric_limits<unsigned int>::max());

                if (specularChance < material.getSpecularIntensity()) {
                    newRayDir = material.findSpecularBounceDirection(ray_, normal,
                                                                     static_cast<unsigned int> (u * v * randSeed *
                                                                                                456789)).Norm();
                    surfaceCol = material.getSpecularColor();
                } else {
                    newRayDir = material.findDiffuseBounceDirection(ray_, normal,
                                                                    static_cast<unsigned int> (u * v * randSeed *
                                                                                               456789)).Norm();
                    surfaceCol = material.getColor();
                }

                // Update ray color
                R *= (surfaceCol.r / 255.99);
                G *= (surfaceCol.g / 255.99);
                B *= (surfaceCol.b / 255.99);

                // Update ray
                ray_.SetOrigin(point + (normal * 0.001));
                ray_.SetDirection(newRayDir);

            } else {
                // if ray does not hit anything, break the loop
                break;
            }
        }

        // Apply accumulated light to the color and return
        return sf::Color(
                static_cast<int>(255.99 * R) * light.x(),
                static_cast<int>(255.99 * G) * light.y(),
                static_cast<int>(255.99 * B) * light.z()
        );
    }

//    /**
//     * @brief Test ray bounce direction on surface, by translating x, y and z componetnts of vector to r, g, and b
//     *
//     * @param u
//     * @param v
//     * @param scene
//     * @return sf::Color
//     */
//    sf::Color TestBounceDir(double u, double v, Scene &scene);

    /**
     * @brief Get the nearest hit information struct
     * 
     * @param ray 
     * @param scene 
     * @return HitInfo 
     */
    static HitInfo GetNearestHitInfo(const Ray &ray, const Scene &scene) {

        HitInfo hit;
        double minDistance = std::numeric_limits<double>::max();

        for (const auto &object: scene.GetObjects()) {

            //Vector intersectionPoint = object->GetIntersectionPoint(ray);
            // Used for finding the closest intersection to ray origin.
            //double intersectionDistance = intersectionPoint.Distance(ray.GetOrigin());
            double intersectionDistance = object->GetIntersectionDistance(ray);

            if (intersectionDistance < minDistance && intersectionDistance > 0) {

                minDistance = intersectionDistance; // Update

                // Update hit info
                hit.hit = true;
                //hit.point = intersectionPoint;
                hit.point = object->GetIntersectionPoint(ray);
                hit.sMaterial = object->GetMaterial();
                //hit.sNormal = object->Normal(intersectionPoint);
                hit.sNormal = object->Normal(hit.point);

            }
        }

        return hit;
    }

    void UpdateRenderContext(sf::Vector2u &dimensions, Scene &scene) {
        context_ = std::make_shared<RenderContext>(dimensions, scene);
    }

    sf::Image GetLatestImage() {
        std::lock_guard<std::mutex> lock(imageMutex_);
        return image_;
    }

    // Updates texture
    void Renderer() {
        while (true) {
            std::shared_ptr<RenderContext> context = context_;

            std::thread threads[threadCount];
            for (int i = 0; i < threadCount; i++) {
                threads[i] = std::thread(RenderWorker, i, threadCount, context);
            }

            for (int i = 0; i < threadCount; i++) {
                threads[i].join();
            }

            std::lock_guard<std::mutex> lock(imageMutex_);
            std::lock_guard<std::mutex> lock2(context->mutex);
            context->frameCount++;
            image_ = context->image;
        }
    }

    static void
    RenderWorker(int threadIndex, int threadCount, std::shared_ptr<RenderContext> context) {
        std::mutex &mutex = context->mutex;

        mutex.lock();
        auto dimensions = context->dimensions;
//        auto &colorBuffer = context->colorBuffer;
        auto scene = context->scene;
        auto frameCount = context->frameCount;
        mutex.unlock();

        double aspectRatio = (double) dimensions.y / dimensions.x;
        for (unsigned int x = threadIndex; x < dimensions.x; x += threadCount) {
            for (unsigned int y = 0; y < dimensions.y; y++) {
                double scaledX = ((double) x * 2 / (double) dimensions.x - 1) * std::min(1.0, aspectRatio);
                double scaledY = (-((double) y * 2 / (double) dimensions.y) + 1) / std::max(1.0, aspectRatio);

                sf::Color nextColor = GetPixelColor(scaledX, scaledY, scene, x * y * frameCount, 12);

                std::lock_guard<std::mutex> lock(mutex);

                sf::Vector2u pixel(x, y);
                sf::Color previousColor = context->image.getPixel(pixel);

                if (frameCount == 0) {
                    context->image.setPixel(pixel, nextColor);
                } else {
                    context->image.setPixel(pixel,
                                            sf::Color((previousColor.r * (frameCount) + nextColor.r) / (frameCount + 1),
                                                      (previousColor.g * (frameCount) + nextColor.g) / (frameCount + 1),
                                                      (previousColor.b * (frameCount) + nextColor.b) /
                                                      (frameCount + 1)));
                }


//                sf::Color pixelColor = GetPixelColor(scaledX, scaledY, scene, x * y * frameCount, 12);
//                sf::Vector3f currentColor(pixelColor.r, pixelColor.g, pixelColor.b);
//
//                // Accumulate color
//                unsigned int bufferIndex = y * dimensions.x + x;
//                mutex.lock();
//                colorBuffer[bufferIndex] += currentColor;
//
//                // average color over the frames
//                sf::Vector3f averagedColor = colorBuffer[bufferIndex] / static_cast<float>(frameCount + 1);
//
//                context->image.setPixel(sf::Vector2u(x, y),
//                                        sf::Color(averagedColor.x, averagedColor.y, averagedColor.z));
//                mutex.unlock();
            }
        }
    }
};

#endif // PATHTRACER_HPP





