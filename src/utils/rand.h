/**
 * @file rand.h
 * @authors Markus Lång
 * @brief Random number generation. Set of functions used for ray bounce direction
 * @version 0.1
 * @date 2023-11-08
 */


#include <random>
#include <iostream>
#include "vector.hpp"

namespace Random {
    

    Vector RayScalar(double a, double b){

        /**
         * @brief Returns a double between a and b to scale ray direction vector.
         * 
         */
        double x, y, z;

        static std::default_random_engine generator(unsigned(time(nullptr)));
        std::uniform_real_distribution<double> distribution(a, b);

        x = distribution(generator);
        y = distribution(generator);
        z = distribution(generator);

        return Vector(x, y, z);
        
    }

} // namespace Random