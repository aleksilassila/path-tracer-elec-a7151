#include "sphere.hpp"

namespace object {
    Sphere::Sphere(const Vector &origin, double radius) : Object(origin), radius_(radius) {}

    Sphere::Sphere(const Vector &origin, double radius, const Material &material): Object(origin, material), radius_(radius) { }

    //Returns distance if it is positive.
    double Sphere::GetIntersectionDistance(const Ray &ray) {
        Vector or_or = GetOrigin() - ray.GetOrigin();
        double b = ray.GetDirection() * or_or;
        double det = b * b - or_or * or_or + radius_ * radius_;

        if (det < 0) return 0;
        else det = sqrt(det);

        double distance;
        return (distance = b - det) > 0 ? distance : ((distance = b + det) > 0 ? distance : -1);
    }

    // normal vector n = ||point - origin||
    // Returns unit vector
    Vector Sphere::Normal(const Vector &point) {
        Vector point_ = point;
        Vector normal = point_ - origin_;
        return normal.Norm();
    }

} // Object namespace