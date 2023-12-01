#ifndef PATHTRACER_MATERIALBUILDER_HPP
#define PATHTRACER_MATERIALBUILDER_HPP

#include <SFML/Graphics.hpp>
#include "vector.hpp"
#include "material.hpp"

/**
 * The builder design pattern to keep Material class encapsulated
 */

// A tiny bit unnecessary (simple setters would've been enough)
class MaterialBuilder {
public:

    /**
     * Constructor with default values
     */
    MaterialBuilder() :
            color_(sf::Color::Magenta),
            roughness_(0.5),
            specularIntensity_(0.5),
            specularColor_(sf::Color::White),
            emission_(Vector()),
            n_(1),
            name_("default") {}

    ~MaterialBuilder() = default;

    inline Material BuildMaterial() const {
        return Material(color_, roughness_, specularIntensity_,
                        specularColor_, emission_, n_, name_);
    }

    inline void SetColor(const sf::Color &color) { color_ = color; }
    inline void SetRoughness(double roughness) { roughness_ = roughness; }
    inline void SetSpecularIntensity(double specularIntensity) { specularIntensity_ = specularIntensity; }
    inline void SetSpecularColor(const sf::Color &specularColor) { specularColor_ = specularColor; }
    inline void SetEmission(const Vector &emission) { emission_ = emission; }
    inline void SetN(double n) { n_ = n; }
    inline void SetName(const std::string& name) { name_ = name; }

private:
    sf::Color color_;
    double roughness_;
    double specularIntensity_;
    sf::Color specularColor_;
    Vector emission_;
    double n_;
    std::string name_;
};


#endif //PATHTRACER_MATERIALBUILDER_HPP
