#include "doctest.h"
#include "../src/world/Objects/objects.hpp"
#include "../src/world/objects/triangle.hpp"


TEST_CASE("Testing getters") {
    object::Triangle triangle = object::Triangle(
            Vector(0, 1, 0),
            Vector(3, -1, 5),
            Vector(1, 0, 4),
            Material()
    );
    CHECK((triangle.GetOrigin() == Vector(0, 1, 0)));
    CHECK((triangle.GetMaterial().GetName() == "default"));
    CHECK((triangle.GetColor() == sf::Color::Magenta));
}

TEST_CASE("Testing normal") {
    object::Triangle triangle = object::Triangle(
            Vector(0, 1, 0),
            Vector(3, -1, 5),
            Vector(1, 0, 4),
            Material()
    );
    CHECK((triangle.Normal(Vector(2, 1, 5)) == Vector(-0.39056673294247168, -0.91132237686576723, -0.13018891098082389)));
}


TEST_CASE("Testing intersection distance") {
    object::Triangle triangle = object::Triangle(
            Vector(0, 1, 0),
            Vector(3, -1, 2),
            Vector(1, 0, 4),
            Material()
    );
    Ray ray = Ray(Vector(2, -3, 2), Vector(1, 3, 3).Norm());
    double d = triangle.GetIntersectionDistance(ray);
}

TEST_CASE("Testing intersection point") {
    object::Triangle triangle = object::Triangle(
            Vector(0, 1, 0),
            Vector(3, -1, 2),
            Vector(1, 0, 4),
            Material()
    );
    Ray ray = Ray(Vector(2, -3, 2), Vector(1, 3, 3).Norm());
    Vector v = triangle.GetIntersectionPoint(ray);
}
