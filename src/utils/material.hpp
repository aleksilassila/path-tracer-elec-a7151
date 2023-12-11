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
    double specularIntensity_;      // How likely rays are to reflect specularly vs diffusely (0 to 1)
    sf::Color specularColor_;       // Specular reflections color.
    Vector emission_;
    std::string name_;
    double n_;                      // Refractive index

public:
    explicit Material(
            sf::Color color = sf::Color(255,0,255),
            double roughness = 0.5,
            double specularIntensity = 0.5,
            sf::Color specularColour = sf::Color(255,255,255),
            Vector emission = Vector(),
            double n = 1,
            const std::string& name = "default"
    );

    ~Material() = default;

    inline sf::Color GetColor() const { return color_; }
    inline double GetRoughness() const { return roughness_; }
    inline double GetSpecularIntensity() const { return specularIntensity_; }
    inline sf::Color GetSpecularColor() const { return specularColor_; }
    inline Vector GetEmission() const { return emission_; }
    inline double GetN() const { return n_; }
    inline const std::string& GetName() const { return name_; }

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
    Vector FindSpecularBounceDirection(Ray &ray, Vector &normal, unsigned int randSeed) const;

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
    Vector FindDiffuseBounceDirection(Vector &normal, unsigned int randSeed) const;

    /**
     * Calculates the odds of reflection and based on those odds plays dice if ray will be reflected.
     * If reflected, returns a zero vector, and new ray direction will be decided by other methods.
     * Otherwise finds refraction direction using Snells'law.
     *
     * @param ray
     * @param normal
     * @return Vector
     */
    Vector FindRefractionDirection(Ray &ray, Vector &normal) const;

};

#endif // MATERIAL_HPP