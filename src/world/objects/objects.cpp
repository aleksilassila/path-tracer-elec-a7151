#include <iostream>
#include "objects.hpp"

namespace object {

    Vector Object::GetIntersectionPoint(const Ray &ray) {
        return ray.GetOrigin() + ray.GetDirection().Norm() * GetIntersectionDistance(ray);
    }

    std::ostream &operator<<(std::ostream &os, const Object &obj) {
        os << "origin: " << obj.origin_;
        return os;
    }

} // Object namespace