#ifndef PATHTRACER_SCENE_H
#define PATHTRACER_SCENE_H

#include <vector>
#include <memory>
#include "Objects/objects.hpp"
#include "camera.h"

/**
 * Class for implementing a scene
 * It bundles the camera and objects together
 */
class Scene {
public:
    Scene() = default;

    Scene(Camera &camera, std::initializer_list<std::shared_ptr<object::Object>> objects)
        : camera_(camera), objects_(objects) {}
    ~Scene() = default;

    Camera &GetCamera() { return camera_; }
    [[nodiscard]] std::vector<std::shared_ptr<object::Object>> GetObjects() const;
    Vector GetAmbientLightDirection() const { return ambientLightDirection_; }

    void SetAmbientLightDirection(Vector direction) { ambientLightDirection_ = direction.Norm(); }

    void AddObject(std::shared_ptr<object::Object> object);

    void RemoveObject(std::shared_ptr<object::Object> object);

    friend std::ostream &operator<<(std::ostream &os, const Scene &scene);

private:
    Camera camera_;
    std::vector<std::shared_ptr<object::Object>> objects_;
    Vector ambientLightDirection_ = Vector(1, -1, -0.5).Norm();
};

#endif //PATHTRACER_SCENE_H
