//
// Created by Aleksi Lassila on 5.11.2023.
//

#include "scene.h"

void Scene::AddObject(std::shared_ptr<Object::Object> object) {

}

void Scene::RemoveObject(std::shared_ptr<Object::Object> object) {

}

std::vector<std::shared_ptr<Object::Object>> Scene::GetObjects() const {
    return {};
}

std::ostream &operator<<(std::ostream &os, const Scene &scene) {
    os << "Scene objects:" << std::endl;

    for (auto &object: scene.objects_) {
        os << *object << std::endl;
    }

    return os;
}

Camera Scene::GetCamera() const {
    return camera_;
}
