#ifndef PATHTRACER_PARALLELOGRAM_HPP
#define PATHTRACER_PARALLELOGRAM_HPP

#include "objects.hpp"

namespace object {
    /**
     * Parallelogram class inherits from Object
     */
    class Parallelogram : public Object {

    private:

        Vector origin_;
        Vector a_;
        Vector b_;
        Vector oa_; // a_ - origin_
        Vector ob_; // b_ - origin_
        Vector oa_ob_; // o_a cross o_b
        Vector n_;

    public:

        Parallelogram(const Vector &origin, const Vector &a, const Vector &b, const Material &material);

        ~Parallelogram() override = default;

        /**
        * Calculates intersection of ray and parallelogram if it exists
        */
        double GetIntersectionDistance(const Ray &ray) override;

        /**
        * Calculates normal vector of parallelogram at point given by vector point
        */
        Vector Normal(const Vector &point) override;

    };

} // Object namespace

#endif //PATHTRACER_PARALLELOGRAM_HPP
