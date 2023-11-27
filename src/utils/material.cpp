#include "material.hpp"
#include "rand.h"

Material::Material(sf::Color color, double roughness, double specularIntensity, sf::Color specularColour, Vector emission, double n, const std::string& name) :
        color_(color),
        roughness_(roughness),
        specularIntensity_(specularIntensity),
        specularColor_(specularColour),
        emission_(emission),
        name_(name),
        n_(n) {}

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
    double n = n_;
    Vector ray_dir = ray.GetDirection().Norm();
    double dot_pro = ray_dir * normal;
    if (dot_pro > 0) {
        n = 1 / n;
        normal = normal * -1;
    }

    double theta1 = std::acos(abs(dot_pro));
    double temp = std::sin(theta1) / n;

    if (temp > 1) {     // Angle bigger than critical angle
        return {Vector(0, 0, 0)};
    }
    double theta2 = std::asin(temp);

    // Parallel and perpendicular reflection coefficients for calculating the odds of reflection vs refraction
    double R_parallel = std::sin(theta1 - theta2) / std::sin(theta1 + theta2);
    double R_perpendicular = std::tan(theta1 - theta2) / std::tan(theta1 + theta2);

    double total_refl = 0.5 * (R_parallel * R_parallel + R_perpendicular * R_perpendicular);
    double x = rand() % 1000;
    if (x < (total_refl * 1000)) return {Vector(0, 0, 0)};

    Vector surface_dir = (ray_dir + (normal * abs(dot_pro))).Norm();
    Vector refr_dir = normal * -std::cos(theta2) +
           surface_dir * std::sin(theta2);
    return {refr_dir};
}