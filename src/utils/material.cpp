#include "material.hpp"
#include "rand.h"

Material::Material(sf::Color color, double roughness, double specularIntensity, sf::Color specularColour, Vector emission, const std::string& name) :
        color_(color),
        roughness_(roughness),
        specularIntensity_(specularIntensity),
        specularColor_(specularColour),
        emission_(emission),
        name_(name) {}

/*
 * Calculate ideal bounce direction based on the reflection formula:
 *      U = v − 2 * (v * n) * n
 * Then generate a random vector based on the ideal bounce and roughness
 */
Vector Material::findSpecularBounceDirection(Ray &ray, Vector &normal, unsigned int randSeed) const {
    Vector rayDirection = ray.GetDirection().Norm();
    Vector idealBounceDirection = rayDirection - (normal * (rayDirection * normal) * 2);

    Vector randomizer = Vector(
            Random::GetRandomDoubleNormal(roughness_, idealBounceDirection.x(), randSeed * std::numeric_limits<unsigned int>::max() * 12314),
            Random::GetRandomDoubleNormal(roughness_, idealBounceDirection.y(), randSeed * std::numeric_limits<unsigned int>::max() * 73495),
            Random::GetRandomDoubleNormal(roughness_, idealBounceDirection.z(), randSeed * std::numeric_limits<unsigned int>::max() * 91247)
    );

    Vector bounceDir = idealBounceDirection + randomizer;
    if (bounceDir.Dot(normal) < 0) return bounceDir * -1;
    return bounceDir;
}

Vector Material::findDiffuseBounceDirection(Vector &normal, unsigned int randSeed) const {
    Vector inHemisphere(
            Random::GetRandomDoubleNormal(roughness_, normal.x(), randSeed * std::numeric_limits<unsigned int>::max() * 12314),
            Random::GetRandomDoubleNormal(roughness_, normal.y(), randSeed * std::numeric_limits<unsigned int>::max() * 73495),
            Random::GetRandomDoubleNormal(roughness_, normal.z(), randSeed * std::numeric_limits<unsigned int>::max() * 91247)
    );

    if (inHemisphere.Dot(normal) < 0) return inHemisphere * -1;
    return inHemisphere;
}

Vector Material::findRefractionDirection(Ray &ray, Vector &normal) const {

   /**
    * todo Implement refraction
    *
    */
    return {};
}