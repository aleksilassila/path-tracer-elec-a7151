#include <iostream>
#include "../../src/utils/vector.hpp"

int main() {
    Vector v = Vector(1, 2, 3);
    Vector v1 = Vector(2, 3, 4);
    Vector v2 = Vector(1, 0, 0);
    Vector v3 = Vector(0, 1, 0);
    Vector v4 = Vector(4, 5, 6);

    std::cout << v << std::endl;
    std::cout << v + v1 << std::endl;
    std::cout << v * 4 << std::endl;
    std::cout << v * v1 << std::endl;
    std::cout << v % v1 << std::endl;
    std::cout << v2 % v3 << std::endl;
    std::cout << v3 % v2 << std::endl;
    std::cout << v2 % v2 << std::endl;
    std::cout << v % v4 << std::endl;
    std::cout << v.Norm() << std::endl;
    std::cout << v4.Norm() << std::endl;
    std::cout << v4 << std::endl;
    std::cout << v.Len() << std::endl;

    return 0;
}
