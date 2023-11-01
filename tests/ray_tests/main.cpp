#include <iostream>
#include "../../src/vector.hpp"
#include "../../src/ray.hpp"

int main() {
    Vector v = Vector(1,2,3);
    Vector v1 = Vector(2,3,4);
    Vector v2 = Vector(1,0,0);
    Vector v3 = Vector(0,1,0);
    Vector v4 = Vector(4,5,6);

	Ray r1 = Ray(v1, v2);
	std::cout << r1.GetOrigin() << std::endl;
	std::cout << r1.GetDirection() << std::endl;

	r1.UpdateOrigin(v3);
	r1.UpdateDirection(v4.Norm());

	std::cout << r1.GetOrigin() << std::endl;
	std::cout << r1.GetDirection() << std::endl;
    return 0;
}
