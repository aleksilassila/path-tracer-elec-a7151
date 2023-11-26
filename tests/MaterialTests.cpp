#include "doctest.h"

#include "../src/utils/material.hpp"
#include "../src/utils/materialbuilder.hpp"

// todo add tests for finding direction

TEST_CASE("Testing getters") {
    auto material = Material();
    CHECK((material.getName() == "default"));
    CHECK((material.getColor() == sf::Color::Magenta));
    CHECK((material.getRoughness() == 0.5));
    CHECK((material.getSpecularIntensity() == 0.5));
    CHECK((material.getSpecularColor() == sf::Color::White));
    CHECK((material.getEmission() == Vector(0,0,0)));
}

TEST_CASE("Testing default builder and getters") {
    auto builder = MaterialBuilder();
    auto material = builder.buildMaterial();
    CHECK((material.getName() == "default"));
    CHECK((material.getColor() == sf::Color::Magenta));
    CHECK((material.getRoughness() == 0.5));
    CHECK((material.getSpecularIntensity() == 0.5));
    CHECK((material.getSpecularColor() == sf::Color::White));
    CHECK((material.getEmission() == Vector(0,0,0)));
}

TEST_CASE("Testing builder") {
    auto builder = MaterialBuilder();
    builder.setName("test");
    builder.setColor(sf::Color::Black);
    builder.setRoughness(0.0);
    builder.setSpecularIntensity(1.0);
    builder.setSpecularColor(sf::Color::Red);
    builder.setEmission(Vector(1.0, 1.0, 1.0));
    auto material = builder.buildMaterial();

    CHECK((material.getName() == "test"));
    CHECK((material.getColor() == sf::Color::Black));
    CHECK((material.getRoughness() == 0.0));
    CHECK((material.getSpecularIntensity() == 1.0));
    CHECK((material.getSpecularColor() == sf::Color::Red));
    CHECK((material.getEmission() == Vector(1.0, 1.0, 1.0)));

    CHECK_FALSE((material.getName() == "default"));
    CHECK_FALSE((material.getColor() == sf::Color::Magenta));
    CHECK_FALSE((material.getRoughness() == 0.5));
    CHECK_FALSE((material.getSpecularIntensity() == 0.5));
    CHECK_FALSE((material.getSpecularColor() == sf::Color::White));
    CHECK_FALSE((material.getEmission() == Vector(0,0,0)));
}