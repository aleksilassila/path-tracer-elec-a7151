/**
 * @file material.cpp
 * @authors Markus Lång
 * @brief Material class implementation
 * @version 0.1
 * @date 2023-11-08
 */

#include "material.hpp"
#include "rand.h"

Material::Material() 
    : colour_(sf::Color::Magenta), roughness_(0.5), emission_(0.0), transparency_(0.0), specularIntensity_(0.5), specularColour_(sf::Color::White) {}

Material::Material(sf::Color colour, double roughness, double specularIntensity, double transparency) 
    : colour_(colour), roughness_(roughness), transparency_(transparency), emission_(0.0), specularIntensity_(specularIntensity), specularColour_(sf::Color::White) {}

    Material::Material(sf::Color colour, double roughness, double specularIntensity, double transparency, sf::Color specularColor) 
    : colour_(colour), roughness_(roughness), transparency_(transparency), specularIntensity_(specularIntensity), specularColour_(specularColor){ }


Material::Material(sf::Color colour, double roughness, double specularIntensity, double transparency, sf::Color specularColor, Vector emission) 
    : colour_(colour), roughness_(roughness), emission_(emission), transparency_(transparency), specularIntensity_(specularIntensity), specularColour_(specularColor){ }

Material::~Material() { }

sf::Color Material::getColor() const { return colour_; }
sf::Color Material::getSpecularColor() const { return specularColour_; }
double Material::getRoughness() const { return roughness_; }
double Material::getSpecularIntensity() const { return specularIntensity_; }
double Material::getTransparency() const { return transparency_; }
Vector Material::getEmission() const { return emission_; }


Vector Material::findSpecularBounceDirection(Ray &ray, Vector &normal, unsigned int randSeed) const {
    
    // Get normalized ray directions
    Vector rayDirection = ray.GetDirection().Norm();

    // Calculate ideal bounce direction based on the reflection formula
    // Reflection formula: U = v − 2 * (v * n) * n
    Vector idealBounceDirection = rayDirection - (normal * (rayDirection * normal) * 2);
    // Generate a random vector based on the ray direction and roughness
    Vector randomizer;
    Vector bounceDir;
    double x, y, z;

    x = Random::GetRandomDoubleNormal(roughness_, idealBounceDirection.x(), randSeed * std::numeric_limits<unsigned int>::max() * 12314);
    y = Random::GetRandomDoubleNormal(roughness_, idealBounceDirection.y(), randSeed * std::numeric_limits<unsigned int>::max() * 73495);
    z = Random::GetRandomDoubleNormal(roughness_, idealBounceDirection.z(), randSeed * std::numeric_limits<unsigned int>::max() * 91247);

    randomizer = Vector(x, y, z);
    bounceDir = idealBounceDirection + randomizer;
    
    if (bounceDir.Dot(normal) < 0){
        return bounceDir * -1;
    }

    return bounceDir;
}

Vector Material::findDiffuseBounceDirection(Ray &ray, Vector &normal, unsigned int randSeed) const {

    // Random unit vector in hemisphere
    double x, y, z;
    x = Random::GetRandomDoubleNormal(roughness_, normal.x(), randSeed * std::numeric_limits<unsigned int>::max() * 12314);
    y = Random::GetRandomDoubleNormal(roughness_, normal.y(), randSeed * std::numeric_limits<unsigned int>::max() * 73495);
    z = Random::GetRandomDoubleNormal(roughness_, normal.z(), randSeed * std::numeric_limits<unsigned int>::max() * 91247);
    
    Vector inHemisphere(x, y, z);
    if (inHemisphere.Dot(normal) < 0) {
        return inHemisphere * -1;
    }

    return inHemisphere;
}

Vector Material::findRefractionDirection(/* Ray &ray, Vector &normal*/) const {

   /**
    * @todo Implement refraction
    * 
    */
}