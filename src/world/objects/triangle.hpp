#ifndef PATHTRACER_TRIANGLE_HPP
#define PATHTRACER_TRIANGLE_HPP

#include "objects.hpp"

namespace object {
    /**
     * Triangle class that inherits from Object
     */
    class Triangle: public Object {

    private:

        Vector origin_;
        Vector a_;
        Vector b_;
        Vector oa_; // a_ - origin_
        Vector ob_; // b_ - origin_
        Vector oa_ob_; // o_a cross o_b
        Vector n_;

    public:

        Triangle(const Vector &origin, const Vector &a, const Vector &b, const Material &material);
        ~Triangle() override = default;

        /**
        * Calculates intersection of ray and triangle if it exists
        */
        double GetIntersectionDistance(const Ray &ray) override;

        /**
        * Calculates normal vector of triangle at point given by vector point
        */
        Vector Normal(const Vector &point) override;

    };

} // Object namespace

#endif //PATHTRACER_TRIANGLE_HPP
