// Material class to handle ray interaction with object

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

public:

    Material();
    Material(sf::Color colour, double roughness);
    ~Material();


    sf::Color getColor() const;
    /**
    *  Get material colour information
    */
    
    Vector findBounceDirection(Ray &ray, Vector &normal);
    /**
    * Find the bounce direction of ray on object based on normal vector of surface
    * Ray must reflect within 180 degrees of normal vector
    */
};


#endif // MATERIAL_HPP