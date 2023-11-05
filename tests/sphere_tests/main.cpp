#include <iostream>
#include "../../src/utils/vector.hpp"
#include "../../src/utils/ray.hpp"
#include "../../src/world/objects.hpp"

int main() {
    Vector v_o = Vector(0, 0, 0), v_d = Vector(3, 1, 1).Norm();
    Vector center = Vector(6, 0, 0);
    Object::Sphere sph = Object::Sphere(center, 3);
    std::cout << sph.getOrigin() << std::endl;
    std::cout << sph.getOrigin() << std::endl;
    Ray ray = Ray(v_o, v_d);
    std::cout << ray.GetDirection() << std::endl;
    std::cout << ray.GetOrigin() << std::endl;

    double distance = sph.Intersect(ray);
    std::cout << "distance: " << distance << std::endl;
    if (distance > 0) {
        Vector intersection = ray.GetOrigin() + ray.GetDirection() * distance;
        std::cout << intersection << std::endl;
        Vector norm = sph.Normal(intersection);
        std::cout << "Normal vector in intersection point is: " << norm << std::endl;
    } else {
        std::cout << "No intersection." << std::endl;
    }

    return 0;
}