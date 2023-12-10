#include "triangle.hpp"

namespace object {

    Triangle::Triangle(const Vector &origin, const Vector &a, const Vector &b, const Material &material) :
            Object(origin, material), a_(a), b_(b) {
        origin_ = origin;
        oa_ = a_ - origin_;
        ob_ = b_ - origin_;
        oa_ob_ = oa_ % ob_;
        n_ = oa_ob_.Norm();
    }

    //This does actually not need a point, have to think about this..
    Vector Triangle::Normal(const Vector &point) { return n_; }

    double Triangle::GetIntersectionDistance(const Ray &ray) {
        Vector dir = ray.GetDirection();
        double det = dir * (oa_ob_);

        if ((-0.0001 < det) && (det < 0.0001)) return 0;

        Vector or_or = ray.GetOrigin() - origin_;
        Vector temp = or_or * (1 / det);
        double u = ob_ % dir * temp;

        if (u < 0 || u > 1) return 0;
        double v = dir % oa_ * temp;

        if (v < 0 || v > 1 || u + v > 1) return 0;
        double t = -(oa_ob_ * temp);

        return t > 0 ? t : 0;
    }
} // Object namespace