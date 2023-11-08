/*
* Abstract object class
*/

#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "../utils/vector.hpp"
#include "../utils/ray.hpp"
#include "../utils/material.hpp"

#include "SFML/Graphics/Color.hpp"

namespace Object {

    class Object {

/**
* abstract Object class
*/

    protected:

        Vector origin_;
        Material material_;

    public:

        Object(Vector origin) : origin_(origin) {}

        Object(Vector origin, const Material &material) : origin_(origin), material_(material) {}

        virtual ~Object() = default;

        Vector getOrigin() const { return origin_; }

        Vector GetIntersectionPoint(const Ray &ray);

        virtual double GetIntersectionDistance(const Ray &ray) = 0;

        virtual Vector Normal(const Vector &point) = 0;

        friend std::ostream &operator<<(std::ostream &os, const Object &obj);

        sf::Color GetColor() const;

        Material GetMaterial() const;
    };

    class Sphere : public Object {

/*
* Sphere class inherits from Object
*/

    private:

        double radius_;

    public:

        Sphere(const Vector &origin, double radius);

        Sphere(const Vector &origin, double radius, const Material &material);

        ~Sphere() override = default;

        /*
        * Calculates intersection of Ray and Sphere if it exists
        */
        double GetIntersectionDistance(const Ray &ray) override;

        Vector Normal(const Vector &point) override;
        /*
        * Calculates normal vector of sphere at point given by vector point
        */

    };


    class Triangle : public Object {

/*
* Triangle class inherits from Object
*/

    private:

        Vector origin_;
        Vector a_;
        Vector b_;
        Vector oa_; // a_ - origin_
        Vector ob_; // b_ - origin_
        Vector oa_ob_; // o_a cross o_b
        Vector n_;

    public:

        Triangle(const Vector &origin, const Vector &a, const Vector &b);

        ~Triangle() override = default;

        /*
        * Calculates intersection of Ray and Triangle if it exists
        */
        double GetIntersectionDistance(const Ray &ray) override;

        Vector Normal(const Vector &point) override;
        /*
        * Calculates normal vector of triangle at point given by vector point
        */

    };

} // namespace Object

#endif // OBJECTS_HPP