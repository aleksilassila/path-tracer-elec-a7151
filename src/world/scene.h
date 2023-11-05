//
// Created by Aleksi Lassila on 4.11.2023.
//

#ifndef PATHTRACER_SCENE_H
#define PATHTRACER_SCENE_H

#include <vector>
#include <memory>
#include "objects.hpp"
#include "camera.h"

class Scene {
public:
    Scene() = default;

    Scene(Camera &camera, std::initializer_list<std::shared_ptr<Object::Object>> objects) : camera_(camera),
                                                                                            objects_(objects) {}

    ~Scene() = default;

    void AddObject(std::shared_ptr<Object::Object> object);

    void RemoveObject(std::shared_ptr<Object::Object> object);

    [[nodiscard]] std::vector<std::shared_ptr<Object::Object>> GetObjects() const;

    Camera GetCamera() const;

    friend std::ostream &operator<<(std::ostream &os, const Scene &scene);

private:
    Camera camera_;
    std::vector<std::shared_ptr<Object::Object>> objects_;
};

#endif //PATHTRACER_SCENE_H
