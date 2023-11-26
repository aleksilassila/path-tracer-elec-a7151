#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <SFML/Graphics.hpp>
#include "vector.hpp"
#include "ray.hpp"

/**
 * Material class to gather material information and handle ray interaction with objects
 */
class Material {

private:
    sf::Color color_;
    double roughness_;
    double specularIntensity_;     // How likely rays are to reflect specularly vs diffusely (0 to 1)
    sf::Color specularColor_;     // Specular reflections color.
    Vector emission_;
    std::string name_;

public:
    explicit Material(
            sf::Color color = sf::Color::Magenta,
            double roughness = 0.5,
            double specularIntensity = 0.5,
            sf::Color specularColour = sf::Color::White,
            Vector emission = Vector(),
            const std::string& name = "default"
    );

    ~Material() = default;

    inline sf::Color getColor() const { return color_; }
    inline double getRoughness() const { return roughness_; }
    inline double getSpecularIntensity() const { return specularIntensity_; }
    inline sf::Color getSpecularColor() const { return specularColor_; }
    inline Vector getEmission() const { return emission_; }
    inline const std::string& getName() const { return name_; }


    /**
     * @brief
     * Find the bounce direction after a ray hits an object based on a normal vector of a surface
     * Ray must reflect within 90 degrees of the normal vector
     *
     * @param ray
     * @param normal
     * @param randSeed
     * @return Vector
     */
    Vector findSpecularBounceDirection(Ray &ray, Vector &normal, unsigned int randSeed) const;

    /**
    * @brief
    * Find the bounce direction after a ray hits an object based on a normal vector of a surface
    * Ray must reflect within 90 degrees of normal vector
    *
    * @param ray
    * @param normal
    * @param randSeed
    * @return Vector
    */
    Vector findDiffuseBounceDirection(Vector &normal, unsigned int randSeed) const;

    /**
     * todo write description
     *
     * @param ray
     * @param normal
     * @return
     */
    Vector findRefractionDirection(Ray &ray, Vector &normal) const;

};

#endif // MATERIAL_HPP