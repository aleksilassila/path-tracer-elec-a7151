/**
 * @file material.cpp
 * @authors Markus Lång
 * @brief Material class implementation
 * @version 0.1
 * @date 2023-11-08
 */

#include "material.hpp"
#include "rand.h"

Material::Material() : colour_(sf::Color::Magenta), roughness_(0.5), emission_(0.0) {}
Material::Material(sf::Color colour, double roughness) : colour_(colour), roughness_(roughness), emission_(0.0) {}
Material::Material(sf::Color colour, double roughness, double emission) : colour_(colour), roughness_(roughness), emission_(emission) {}
Material::~Material() {}

sf::Color Material::getColor() const { return colour_; }
double Material::getRoughness() const { return roughness_; }
double Material::getEmission() const { return emission_; }

Vector Material::findBounceDirection(Ray &ray, Vector &normal) const
{
    // Get normalized ray directions
    Vector rayDirection = ray.GetDirection().Norm();

    // Calculate ideal bounce direction based on the reflection formula
    // Reflection formula: U = v − 2 * (v * n) * n
    Vector idealBounceDirection = rayDirection - (normal * (rayDirection * normal) * 2);
    // Generate a random vector based on the ray direction and roughness
    unsigned int seed = 0;
    Vector randomizer;
    Vector bounceDir;
    double x, y, z;

    x = Random::GetRandomDoubleUniform(-roughness_, roughness_, seed);
    y = Random::GetRandomDoubleUniform(-roughness_, roughness_, seed * 2);
    z = Random::GetRandomDoubleUniform(-roughness_, roughness_, seed * 3);
    randomizer = Vector(x, y, z);
    bounceDir = idealBounceDirection + randomizer;
    seed++;

    return bounceDir;
}