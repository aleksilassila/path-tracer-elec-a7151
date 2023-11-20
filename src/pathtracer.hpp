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

/**
 * This struct is a thread-safe way of representing the data needed to render a frame.
 */
struct RenderContext {
private:
    const Scene scene_;
    const sf::Vector2u dimensions_;

//    const std::vector<sf::Vector3f> colorBuffer;
    sf::Image image_;

    std::mutex mutex;
public:
    std::atomic<int> frameCount = 0;
    std::atomic<int> maxBounces = 12;

    RenderContext(sf::Vector2u &dimensions, Scene &scene) : RenderContext(dimensions, scene, 12) {}

    RenderContext(sf::Vector2u &dimensions, Scene &scene, int maxBounces) : scene_(scene), dimensions_(dimensions),
                                                                            maxBounces(maxBounces) {
        image_.create(dimensions, sf::Color::Transparent);
    }

    sf::Color GetPixel(sf::Vector2u pos) {
        std::lock_guard<std::mutex> lock(mutex);
        return image_.getPixel(pos);
    }

    void SetPixel(sf::Vector2u pos, sf::Color color) {
        std::lock_guard<std::mutex> lock(mutex);
        image_.setPixel(pos, color);
    }

    sf::Image GetImage() {
        std::lock_guard<std::mutex> lock(mutex);
        return image_;
    }

    Scene GetScene() {
        std::lock_guard<std::mutex> lock(mutex);
        return scene_;
    }

    sf::Vector2u GetDimensions() {
        std::lock_guard<std::mutex> lock(mutex);
        return dimensions_;
    }
};

class PathTracer {

private:

    const int threadCount = std::max(1, (int) (std::thread::hardware_concurrency() * 0.8));
    std::shared_ptr<RenderContext> context_;

    sf::Image image_;
    std::mutex imageMutex_;

public:
    PathTracer(sf::Vector2u &dimensions, Scene &scene) : context_(
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

    /**
     * This function runs inside a thread and continuously renders an image based on the rendering context.
     * @param window
     */
    void Renderer(sf::RenderWindow &window) {
        while (window.isOpen()) {
            std::shared_ptr<RenderContext> context = context_;

            std::thread threads[threadCount];
            for (int i = 0; i < threadCount; i++) {
                threads[i] = std::thread(RenderWorker, i, threadCount, context);
            }

            for (int i = 0; i < threadCount; i++) {
                threads[i].join();
            }

            std::lock_guard<std::mutex> lock(imageMutex_);
            image_ = context->GetImage();
            context->frameCount++;
        }
    }

    /**
     * This function renders a specific part of the image inside a thread.
     * @param threadIndex Current thread eg which part of the image to render
     * @param threadCount Amount of threads used
     * @param context Rendering context from which to copy values
     */
    static void
    RenderWorker(int threadIndex, int threadCount, std::shared_ptr<RenderContext> context) {
        auto dimensions = context->GetDimensions();
//        auto &colorBuffer = context->colorBuffer;
        auto scene = context->GetScene();
        int frameCount = context->frameCount;
        int maxBounces = context->maxBounces;

//        double resolution = 0.5;

        double aspectRatio = (double) dimensions.x / dimensions.y;
        for (unsigned int x = threadIndex; x < dimensions.x; x += threadCount) {
            for (unsigned int y = 0; y < dimensions.y; y++) {
//                if (Random::GetRandomDoubleUniform(0.0, 1.0,
//                                                   frameCount) > resolution) {
//                    continue;
//                }

                double scaledX = ((double) x * 2 / (double) dimensions.x - 1) * std::min(1.0, aspectRatio);
                double scaledY = (-((double) y * 2 / (double) dimensions.y) + 1) / std::max(1.0, aspectRatio);

                sf::Color nextColor = GetPixelColor(scaledX, scaledY, scene, x * y * frameCount, maxBounces);

                sf::Vector2u pixel(x, y);

                // Accumulate rays
                if (frameCount == 0) {
                    context->SetPixel(pixel, nextColor);
                } else {
                    sf::Color previousColor = context->GetPixel(pixel);

                    context->SetPixel(pixel,
                                      sf::Color(
                                              (previousColor.r * (frameCount) + nextColor.r) / (frameCount + 1),
                                              (previousColor.g * (frameCount) + nextColor.g) / (frameCount + 1),
                                              (previousColor.b * (frameCount) + nextColor.b) /
                                              (frameCount + 1)));
                }
            }
        }
    }
};

#endif // PATHTRACER_HPP
