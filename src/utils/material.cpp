
#include "material.hpp"

Material::Material(): colour_(sf::Color::Magenta), roughness_(0.0) { }

Material::Material(sf::Color colour, double roughness): colour_(colour), roughness_(roughness) { }

Material::~Material() { }


sf::Color Material::getColor() const { return colour_; }

Vector findBounceDirection(Ray &ray, Vector &normal) {

    // Get normalised ray direction
    Vector rayDirection = ray.GetDirection().Norm();
    // Find bounce direction based on 2 unit vectors
    // U = D − 2(D * n)n
    Vector idealBounceDirection = (rayDirection - 2 * (rayDirection * normal)) * normal;
    // TODO add randomness to bounce direction based on material roughness
    return idealBounceDirection;
    
}