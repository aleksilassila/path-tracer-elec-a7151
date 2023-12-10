#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "../../utils/vector.hpp"
#include "../../utils/ray.hpp"
#include "../../utils/material.hpp"

#include "SFML/Graphics/Color.hpp"

namespace object {

    /**
     * Abstract class (interface) for objects
     */
    class Object {

    protected:
        Vector origin_;
        Material material_;

    public:
        Object(Vector origin) : origin_(origin) {}

        Object(Vector origin, const Material &material) : origin_(origin), material_(material) {}

        virtual ~Object() = default;

        Vector GetOrigin() const { return origin_; }
        Material GetMaterial() const { return material_; }
        sf::Color GetColor() const { return material_.GetColor(); }
        Vector GetIntersectionPoint(const Ray &ray);

        /**
         * Calculated distance between a ray and the object
         * @param ray
         * @return double
         */
        virtual double GetIntersectionDistance(const Ray &ray) = 0;

        /**
         * Calculates normal vector at a given point on the object
         * @param point
         * @return Vector
         */
        virtual Vector Normal(const Vector &point) = 0;

        /**
         * Writes to stream
         * @param stream
         * @param vector
         * @return std::stream
         */
        friend std::ostream &operator<<(std::ostream &os, const Object &obj);
    };

} // Object namespace

#endif // OBJECTS_HPP