#include "doctest.h"

#include "../src/utils/material.hpp"
#include "../src/utils/materialbuilder.hpp"


TEST_CASE("Testing getters") {
    auto material = Material();
    CHECK((material.GetName() == "default"));
    CHECK((material.GetColor() == sf::Color::Magenta));
    CHECK((material.GetRoughness() == 0.5));
    CHECK((material.GetSpecularIntensity() == 0.5));
    CHECK((material.GetSpecularColor() == sf::Color::White));
    CHECK((material.GetEmission() == Vector(0, 0, 0)));
}

TEST_CASE("Testing default builder and getters") {
    auto builder = MaterialBuilder();
    auto material = builder.BuildMaterial();
    CHECK((material.GetName() == "default"));
    CHECK((material.GetColor() == sf::Color::Magenta));
    CHECK((material.GetRoughness() == 0.5));
    CHECK((material.GetSpecularIntensity() == 0.5));
    CHECK((material.GetSpecularColor() == sf::Color::White));
    CHECK((material.GetEmission() == Vector(0, 0, 0)));
}

TEST_CASE("Testing builder") {
    auto builder = MaterialBuilder();
    builder.SetName("test");
    builder.SetColor(sf::Color::Black);
    builder.SetRoughness(0.0);
    builder.SetSpecularIntensity(1.0);
    builder.SetSpecularColor(sf::Color::Red);
    builder.SetEmission(Vector(1.0, 1.0, 1.0));
    auto material = builder.BuildMaterial();

    CHECK((material.GetName() == "test"));
    CHECK((material.GetColor() == sf::Color::Black));
    CHECK((material.GetRoughness() == 0.0));
    CHECK((material.GetSpecularIntensity() == 1.0));
    CHECK((material.GetSpecularColor() == sf::Color::Red));
    CHECK((material.GetEmission() == Vector(1.0, 1.0, 1.0)));

    CHECK_FALSE((material.GetName() == "default"));
    CHECK_FALSE((material.GetColor() == sf::Color::Magenta));
    CHECK_FALSE((material.GetRoughness() == 0.5));
    CHECK_FALSE((material.GetSpecularIntensity() == 0.5));
    CHECK_FALSE((material.GetSpecularColor() == sf::Color::White));
    CHECK_FALSE((material.GetEmission() == Vector(0, 0, 0)));
}