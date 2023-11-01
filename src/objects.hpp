/*
* Abstract object class
*/

#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "vector.hpp"

namespace Object { 

class Object {

/*
* abstract Object class
*/

protected:

    Vector origin_;
    // TODO MATERIAL

public:

    Object(Vector origin): origin_(origin) {}
    virtual ~Object();

    Vector getOrigin() const { return origin_; }

    virtual Vector Intersection(const Vector & ray) = 0;
    virtual Vector Normal(const Vector & point) = 0;

};

class Sphere: public Object {

/*
* Sphere class inherits from Object
*/

private:

    double radius_;

public:

    Sphere(const Vector & origin, double radius): Object(origin), radius_(radius) {}
    ~Sphere() override = default;

    Vector Intersection(const Vector & ray) override;
    /*
    * Calculates intersection of Ray and Sphere if it exists
    */

    Vector Normal(const Vector & point) override;
    /*
    * Calculates normal vector of sphere at point given by vector point
    */

};


} // namespace Object

#endif // OBJECTS_HPP