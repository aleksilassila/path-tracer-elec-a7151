/**
 * @file material.hpp
 * @authors Markus Lång
 * @brief Material class to handle ray interaction with object
 * @version 0.1
 * @date 2023-11-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MATERIAL_HPP
#define MATERIAL_HPP


#include <SFML/Graphics.hpp>
#include "vector.hpp"
#include "ray.hpp"


class Material {

private:

    // Material attributes
    sf::Color colour_;
    double roughness_;
    double emission_;

public:

    Material();
    Material(sf::Color colour, double roughness);
    Material(sf::Color colour, double roughness, double emission);
    ~Material();

    /**
     * @brief Get the Color of Material
     * 
     * @return sf::Color 
     */
    sf::Color getColor() const;


    /**
     * @brief Get the object emission
     * 
     * @return double 
     */

    double getEmission() const;



    /** @brief 
    *  Get material roughness information
    */
   double getRoughness() const;


   /**
    * @brief 
    * Find the bounce direction of ray on object based on normal vector of surface
    * Ray must reflect within 180 degrees of normal vector
    * 
    * @param ray 
    * @param normal 
    * @return Vector 
    */
    Vector findBounceDirection(Ray &ray, Vector &normal) const;

};

#endif // MATERIAL_HPP