/*
* Objects implementation
*/

#include <iostream>
#include "objects.hpp"
#include "../utils/ray.hpp"
#include "../utils/vector.hpp"


Object::Sphere::Sphere(const Vector &origin, double radius) : Object(origin), radius_(radius) {}

//Object::Sphere::~Sphere(){}

Vector Object::Sphere::GetIntersection(const Vector &ray) {
    // TODO 
    return Vector(0, 0, 0);
}

double Object::Sphere::GetIntersectionDistance(Ray &ray) {
    Vector or_or = getOrigin() - ray.GetOrigin();
//    std::cout << "or_or: " << or_or << std::endl;
//    std::cout << "ray or: " << ray.GetOrigin() << std::endl;
//    std::cout << "ray direction: " << ray.GetDirection() << std::endl;
    double b = ray.GetDirection() * or_or;
//    std::cout << "b: " << b << std::endl;
    double det = b * b - or_or * or_or + radius_ * radius_;
//    std::cout << "det: " << det << std::endl;
    if (det < 0) return 0;
    else det = sqrt(det);
//    std::cout << "det: " << det << std::endl;
    double distance;
    //Returns distance if it is positive.
    return (distance = b - det) > 0 ? distance : ((distance = b + det) > 0 ? distance : -1);
    //distance = b - det;
    //if (distance > 0) {
    //    std::cout << "distance: " << distance << std::endl;
    //    return distance;
    //}
    //else distance = b + det; 
    //if (distance > 0) {
    //    std::cout << "distance: " << distance << std::endl;
    //    return distance;
    //}
    //else return 0;
}

Vector Object::Sphere::Normal(const Vector &point) {

    Vector point_ = point;
    // normal vector n = ||point - origin||
    Vector normal = point_ - origin_;
    // Return unit vector
    return normal.Norm();
}


Object::Triangle::Triangle(const Vector &origin, const Vector &a, const Vector &b) : Object(origin), a_(a), b_(b) {
    origin_ = origin;
    oa_ = a_ - origin_;
    ob_ = b_ - origin_;
    oa_ob_ = oa_ % ob_;
    n_ = oa_ob_.Norm();
}

Vector Object::Triangle::Normal(const Vector &point) { return n_; }
//This does actually not need a point, have to think about this...

Vector Object::Triangle::GetIntersection(const Vector &ray) {
    // TODO 
    return Vector(0, 0, 0);
}

double Object::Triangle::GetIntersectionDistance(Ray &ray) {
    Vector dir = ray.GetDirection();
    double det = dir * (oa_ob_);
    std::cout << "oa: " << oa_ << "ob: " << ob_ << "oa_ob: " << oa_ob_ << std::endl;
    std::cout << origin_ << std::endl;
    if (-0.0001 < det < 0.0001) return 0;
    std::cout << "det: " << det << std::endl;
    Vector or_or = ray.GetOrigin() - origin_;
    Vector temp = or_or * (1 / det);
    double u = ob_ % dir * temp;
    std::cout << "u: " << u << std::endl;
    if (u < 0 || u > 1) return 0;
    double v = dir % oa_ * temp;
    std::cout << "v: " << v << std::endl;
    if (v < 0 || v > 1) return 0;
    if (u + v > 1) return 0;
    double t = -(oa_ob_ * temp);
    std::cout << "t: " << t << std::endl;
    return t > 0 ? t : 0;
}

std::ostream &Object::operator<<(std::ostream &os, const Object &obj) {
    os << "origin: " << obj.origin_;
    return os;
}
