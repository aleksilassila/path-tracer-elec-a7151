#ifndef PATHTRACER_SPHERE_HPP
#define PATHTRACER_SPHERE_HPP

#include "objects.hpp"

namespace object {

    /**
     * Sphere class inherits from Object
     */
    class Sphere : public Object {

    private:

        double radius_;

    public:

        Sphere(const Vector &origin, double radius);

        Sphere(const Vector &origin, double radius, const Material &material);

        ~Sphere() override = default;

        /**
        * Calculates intersection of ray and sphere if it exists
        */
        double GetIntersectionDistance(const Ray &ray) override;

        /**
        * Calculates normal vector of sphere at point given by vector point
        */
        Vector Normal(const Vector &point) override;

    };

} // Object namespace

#endif //PATHTRACER_SPHERE_HPP
