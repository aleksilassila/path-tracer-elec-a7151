#include "doctest.h"
#include "../src/world/objects.hpp"


//todo add material, color, radius check later
TEST_CASE("Testing getters") {
    Object::Sphere sphere = Object::Sphere(Vector(0, 0, 0), 3);

    CHECK((sphere.getOrigin() == Vector(0, 0, 0)));
//    CHECK((sphere.GetColor() == sf::Color::White));
//    CHECK((sphere.GetMaterial() == ));
}

TEST_CASE("Testing normal") {
    Object::Sphere sphere = Object::Sphere(Vector(3, 1, 5), 3);

    CHECK((sphere.Normal(Vector(0, 1, 0)) == Vector(-0.51449575542752646,0,-0.8574929257125441)));
}

//todo ask how the functions work
TEST_CASE("Testing intersection") {
    Object::Sphere sphere = Object::Sphere(Vector(0, 0, 0), 3);

    Ray rayA = Ray(Vector(-5, 1, 4), Vector(-5, 1, 1).Norm());
    double dA = sphere.GetIntersectionDistance(rayA);
    Vector vA = sphere.GetIntersectionPoint(rayA);
//    CHECK((sphere.GetIntersectionDistance(rayA) == 0));
//    CHECK((sphere.GetIntersectionPoint(rayA) == rayA.GetOrigin()));

    Ray rayB = Ray(Vector(-5, 1, 4), Vector(3, 2, -2).Norm());
    double dB = sphere.GetIntersectionDistance(rayB);
    Vector vB = sphere.GetIntersectionPoint(rayB);
//    CHECK((sphere.GetIntersectionDistance(rayB) == -1));
//    CHECK((sphere.GetIntersectionPoint(rayB) == Vector(0,0,0)));
}