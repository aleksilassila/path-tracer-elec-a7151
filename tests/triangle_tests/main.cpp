#include <iostream>
#include "../../src/utils/vector.hpp"
#include "../../src/utils/ray.hpp"
#include "../../src/world/objects.hpp"


int main() {
    Vector orig = Vector(0, 1, 0);
    Vector a = Vector(0, -1, 0);
    Vector b = Vector(1, 0, 1);
    Object::Triangle tria = Object::Triangle(orig, a, b);

    Vector o = Vector(1, 0, 0);
    Vector dir = Vector(-1, 0, 0.5).Norm();
    Ray ray = Ray(o, dir);
    double distance = tria.Intersect(ray);
    if (distance == 0) {
        std::cout << "No hit" << std::endl;
    } else {
        std::cout << distance << std::endl;
        Vector intersect_point = o + dir * distance;
        std::cout << "Intersection at: " << intersect_point << std::endl;
    }
    return 0;
}