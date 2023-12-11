#ifndef PATHTRACER_HPP
#define PATHTRACER_HPP

#include <mutex>
#include <atomic>
#include <thread>
#include "SFML/Graphics/Color.hpp"
#include "utils/ray.hpp"
#include "world/scene.h"
#include "utils/rand.h"

#define REAL_TIME_TARGET_FPS 15

/**
 * Struct that keeps information about a hit point
 */
struct HitInfo {
    bool hit; // did ray hit an object
    Vector point; // hit point
    Vector sNormal; // surface normal of point hit
    Material sMaterial; // material of object hit

    HitInfo() : hit(false), point(), sNormal(), sMaterial() {}
};

/**
 * This struct is a thread-safe way of representing the data needed to render a frame.
 */
struct RenderContext {
private:
    const Scene scene_;
    const sf::Vector2u dimensions_;

    std::vector<sf::Vector3f> colorBuffer_ = std::vector<sf::Vector3f>(dimensions_.x * dimensions_.y);
    sf::Image image_;

    std::mutex mutex;
public:
    std::atomic<int> frameCount = 0;
    std::atomic<int> maxBounces = 12;
    std::atomic<bool> cancelled = false;

    RenderContext(sf::Vector2u &dimensions, Scene &scene) : RenderContext(dimensions, scene,
                                                                          12) {}

    RenderContext(sf::Vector2u &dimensions, Scene &scene, int maxBounces)
            : scene_(scene), dimensions_(dimensions), maxBounces(maxBounces) {
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

    sf::Color AverageColor(sf::Vector2u pos, sf::Color color) {
        std::lock_guard<std::mutex> lock(mutex);

        int bufferIndex = pos.y * dimensions_.x + pos.x;
        colorBuffer_[bufferIndex] += sf::Vector3f(color.r, color.g, color.b);

        return sf::Color(
                colorBuffer_[bufferIndex].x / (frameCount + 1),
                colorBuffer_[bufferIndex].y / (frameCount + 1),
                colorBuffer_[bufferIndex].z / (frameCount + 1)
        );
    }
};

/**
 * The core class of this application which defines the rendering logic
 */
class PathTracer {

private:

    const int threadCount = std::max(1, (int) (std::thread::hardware_concurrency() * 0.8));
    std::shared_ptr<RenderContext> context_;

    sf::Image image_;
    std::mutex imageMutex_;

public:
    PathTracer(sf::Vector2u &dimensions, Scene &scene)
            : context_(std::make_shared<RenderContext>(dimensions, scene)) {}

    ~PathTracer() = default;

    sf::Image GetLatestImage() {
        return context_->GetImage();
    }

    void UpdateRenderContext(sf::Vector2u &dimensions, Scene &scene) {
        context_->cancelled = true;
        context_ = std::make_shared<RenderContext>(dimensions, scene);
    }

    static void Draw(sf::RenderWindow &window, sf::Image &image) {
        if (image.getSize().x != 0 && image.getSize().y != 0) {
            sf::Texture texture;
            bool textureDidLoad = texture.loadFromImage(image);
            if (!textureDidLoad) return;

            sf::Sprite sprite(texture);

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }

    /**
     * This function runs inside a thread and continuously renders an image based on the rendering context.
     * @param window
     */
    void Renderer(sf::RenderWindow &window) {
        while (window.isOpen()) {
            std::shared_ptr<RenderContext> context = context_;

            std::vector<int> indexes(context->GetDimensions().x * context->GetDimensions().y);
            std::iota(indexes.begin(), indexes.end(), 0);

            std::random_device rd;
            std::mt19937 g(rd());

            std::shuffle(indexes.begin(), indexes.end(), g);

            std::thread threads[threadCount];
            std::vector<std::atomic<bool>> isDone(threadCount);
            for (int i = 0; i < threadCount; i++) {
                auto threadIndexes = std::vector<int>(indexes.begin() + i * indexes.size() / threadCount,
                                                      indexes.begin() + (i + 1) * indexes.size() / threadCount);

                threads[i] = std::thread(RenderWorker, threadIndexes, context, std::ref(isDone[i]));
            }

            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / REAL_TIME_TARGET_FPS));
                bool allDone = true;
                for (int i = 0; i < threadCount; i++) {
                    if (!isDone[i]) {
                        allDone = false;
                        break;
                    }
                }
                if (allDone) break;
                sf::Image image = context->GetImage();
                Draw(window, image);
            }

            for (int i = 0; i < threadCount; i++) {
                threads[i].join();
            }

            context->frameCount++;
            sf::Image image = context->GetImage();
            Draw(window, image);
        }
    }

private:
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
                if (lastHit_.sMaterial.GetEmission().Len() > 0.0) {
                    // Break out of loop if ray hits a light.
                    light += lastHit_.sMaterial.GetEmission();
                    break;
                }

                Vector point = lastHit_.point;
                Vector normal = lastHit_.sNormal;
                Material material = lastHit_.sMaterial;
                Vector newRayDir = Vector(0, 0, 0);
                sf::Color surfaceCol;

                if (material.GetN() > 1) {
                    newRayDir = material.FindRefractionDirection(ray_, normal);
                    /* findRefractionDirection calculates the odds of reflection and based on those odds plays dice if
                     ray will be reflected. If reflected, returns a zero vector, and newRayDir will be decided by
                     other methods.
                    */
                    if (newRayDir != Vector(0, 0, 0)) {
                        surfaceCol = material.GetColor();
                        // Ray goes in, so its origin will be set inside the object
                        ray_.SetOrigin(point - (normal * 0.001));
                    }
                }
                if (newRayDir == Vector(0, 0, 0)) {
                    if (normal * ray_.GetDirection() > 0) {
                        normal = normal * (-1);
                    }
                    // Handle diffuse and specular reflections
                    double specularChance = Random::GetRandomDoubleUniform(0.0, 1.0,
                                                                           static_cast<unsigned int> (u * v * randSeed *
                                                                                                      456789) *
                                                                           std::numeric_limits<unsigned int>::max());

                    if (specularChance < material.GetSpecularIntensity()) {
                        newRayDir = material.FindSpecularBounceDirection(ray_, normal,
                                                                         static_cast<unsigned int> (u * v * randSeed *
                                                                                                    456789)).Norm();
                        surfaceCol = material.GetSpecularColor();
                    } else {
                        newRayDir = material.FindDiffuseBounceDirection(normal,
                                                                        static_cast<unsigned int> (u * v * randSeed *
                                                                                                   456789)).Norm();
                        surfaceCol = material.GetColor();
                    }
                    ray_.SetOrigin(point + (normal * 0.001));
                }

                // Update ray color
                R *= (surfaceCol.r / 255.99);
                G *= (surfaceCol.g / 255.99);
                B *= (surfaceCol.b / 255.99);

                // Update ray
                //ray_.SetOrigin(point + (normal * 0.001));
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

    /**
     * This function renders a specific part of the image inside a thread.
     * @param threadIndex Current thread eg which part of the image to render
     * @param threadCount Amount of threads used
     * @param context Rendering context from which to copy values
     */
    static void
    RenderWorker(std::vector<int> indexes, std::shared_ptr<RenderContext> context, std::atomic<bool> &isDone) {
        auto dimensions = context->GetDimensions();
        auto scene = context->GetScene();
        int frameCount = context->frameCount;
        int maxBounces = context->maxBounces;

        double aspectRatio = (double) dimensions.x / dimensions.y;
        auto startTime = std::chrono::high_resolution_clock::now();

        for (unsigned int i: indexes) {
            auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - startTime).count();

            if (context->cancelled && timeElapsed > 1000 / REAL_TIME_TARGET_FPS) {
                break;
            }

            unsigned int x = i % dimensions.x;
            unsigned int y = i / dimensions.x;

            double scaledX = ((double) x * 2 / (double) dimensions.x - 1) * std::min(1.0, aspectRatio);
            double scaledY = (-((double) y * 2 / (double) dimensions.y) + 1) / std::max(1.0, aspectRatio);

            sf::Color nextColor = GetPixelColor(scaledX, scaledY, scene, x * y * frameCount, maxBounces);

            sf::Vector2u pixel(x, y);

            auto average = context->AverageColor(pixel, nextColor);

            context->SetPixel(pixel, average);
        }

        isDone = true;
    }
};

#endif // PATHTRACER_HPP
