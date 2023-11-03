/*
* Objects implementation
*/

#include <iostream>
#include "objects.hpp"
#include "ray.hpp"
//#include "vector.hpp"


Object::Sphere::Sphere(const Vector & origin, double radius) : Object(origin), radius_(radius) { }

//Object::Sphere::~Sphere(){}

Vector Object::Sphere::Intersection(const Vector & ray) {
    // TODO 
    return Vector(0,0,0); 
}   

double Object::Sphere::Intersect(Ray & ray) {
    Vector or_or = getOrigin() - ray.GetOrigin(); 
    std::cout << "or_or: " << or_or << std::endl;
    std::cout << "ray or: " << ray.GetOrigin() << std::endl;
    std::cout << "ray direction: " << ray.GetDirection() << std::endl;
    double b = ray.GetDirection() * or_or;
    std::cout << "b: " << b << std::endl;
    double det = b*b - or_or * or_or + radius_*radius_;
    std::cout << "det: " << det << std::endl;
    if (det < 0) return 0;
    else det = sqrt(det);
    std::cout << "det: " << det << std::endl;
    double distance;
    //Returns distance if it is positive. 
    return (distance = b - det) > 0 ? distance : ((distance = b + det) > 0 ? distance : 0);
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

Vector Object::Sphere::Normal(const Vector & point) {
    
    Vector point_ = point;
    // normal vector n = ||point - origin||
    Vector normal = point_ - origin_;
    // Return unit vector
    return normal.Norm();
}
