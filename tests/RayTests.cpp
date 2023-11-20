#include "doctest.h"

#include "../src/utils/vector.hpp"
#include "../src/utils/ray.hpp"

TEST_CASE("Testing getters") {
    Ray ray = Ray(Vector(2, 3, 4), Vector(1, 0, 0));
    Ray rayBlack = Ray(Vector(2, 3, 4), Vector(1, 0, 0), sf::Color(0,0,0));

    CHECK((ray.GetOrigin() == Vector(2, 3, 4)));
    CHECK((ray.GetDirection() == Vector(1, 0, 0)));
    CHECK((ray.GetColor() == sf::Color(255,255,255)));
    CHECK((rayBlack.GetColor() == sf::Color(0,0,0)));
}

TEST_CASE("Testing setters") {
    Ray ray = Ray(Vector(2, 3, 4), Vector(1, 0, 0));

    ray.SetOrigin(Vector(0, 1, 0));
    ray.SetDirection(Vector(4, 5, 6));
    ray.SetColor(sf::Color(0,0,0));

    CHECK((ray.GetOrigin() == Vector(0, 1, 0)));
    CHECK((ray.GetDirection() == Vector(4, 5, 6)));
    CHECK((ray.GetColor() == sf::Color(0,0,0)));
}
