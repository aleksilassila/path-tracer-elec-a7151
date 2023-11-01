/*
* Ohjects implementation
*/

#include <iostream>
#include "objects.hpp"


Object::Sphere::Sphere(const Vector & origin, double radius): Object(origin), radius_(radius) { }

Object::Sphere::~Sphere(){}

Vector Object::Sphere::Intersection(const Vector & ray) {
    // TODO 
}   

Vector Object::Sphere::Normal(const Vector & point) {
    
    Vector point_ = point;
    // normal vector n = ||point - origin||
    Vector normal = point_ - origin_;
    // Return unit vector
    return normal.Norm();
}
