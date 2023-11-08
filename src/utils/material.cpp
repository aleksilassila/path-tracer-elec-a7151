/**
 * @file material.cpp
 * @authors Markus Lång
 * @brief Material class implementation
 * @version 0.1
 * @date 2023-11-08
 */


#include "material.hpp"
#include "rand.h"


Material::Material(): colour_(sf::Color::Magenta), roughness_(0.0), emission_(0.0) { }
Material::Material(sf::Color colour, double roughness): colour_(colour), roughness_(roughness), emission_(0.0) { }
Material::Material(sf::Color colour, double roughness, double emission): colour_(colour), roughness_(roughness), emission_(emission) { }

Material::~Material() { }


sf::Color Material::getColor() const { return colour_; }
double Material::getRoughness() const{ return roughness_; }
double Material::getEmission() const { return emission_;}


Vector Material::findBounceDirection(Ray &ray, Vector &normal) const{

    // Get normalised ray direction
    Vector rayDirection = ray.GetDirection().Norm();
    // Find bounce direction based on 2 unit vectors
    // U = v − 2(v * n)n
    Vector idealBounceDirection = (rayDirection - 2 * (rayDirection * normal)) * normal;
    // Find randomized vector 
    Vector randomizer = Random::RayScalar(-roughness_, +roughness_);
    // add the the two vectors together and compute norm
    Vector bounceDirection = (idealBounceDirection + randomizer).Norm();
    return idealBounceDirection;

}