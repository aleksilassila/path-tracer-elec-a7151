#ifndef RAND_H
#define RAND_H

#include <random>
#include <iostream>
#include "vector.hpp"

/**
 * Random number generation. Contains a set of functions used for ray bounce direction
 */
namespace Random {

    /**
     * @brief Random double with uniform distribution
     * 
     * @return std::default_random_engine 
     */
    double GetRandomDoubleUniform(double min, double max, unsigned int seed);

    /**
    * @brief Random double with normal distribution
    * 
    * @return std::default_random_engine 
    */
    double GetRandomDoubleNormal(double stddev, double average, unsigned int seed);
}

#endif // RAND_H
