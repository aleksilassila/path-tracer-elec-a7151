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

    sf::Color colour_;
    double roughness_;
    double specularIntensity_;

    double transparency_;

    Vector emission_;
    // Specular reflections color.
    sf::Color specularColour_;
    // How likely rays are to reflect specularly vs diffusely (0 to 1)
    


public:

    Material();
    Material(sf::Color colour, double roughness, double specularIntensity, double transparency);
    Material(sf::Color colour, double roughness, double specularIntensity, double transparency, sf::Color speuclarColor, Vector emission);
    ~Material();

    /**
     * @brief Get the Color of Material
     * 
     * @return sf::Color 
     */
    sf::Color getColor() const;

    sf::Color getSpecularColor() const;


    /**
     * @brief Get the object emission
     * 
     * @return double 
     */

    Vector getEmission() const;



    /** @brief 
    *  Get material roughness information
    */
   double getRoughness() const;


   double getSpecularIntensity() const;

double getTransparency() const;



   /**
    * @brief 
    * Find the bounce direction of ray on object based on normal vector of surface
    * Ray must reflect within 180 degrees of normal vector
    * 
    * @param ray 
    * @param normal 
    * @return Vector 
    */
    Vector findSpecularBounceDirection(Ray &ray, Vector &normal) const;
    Vector findDiffuseBounceDirection(Ray &ray, Vector &normal) const;
    Vector findRefractionDirection(/* Ray &ray, Vector &normal */) const;

};

#endif // MATERIAL_HPP