/**
 * @file rand.h
 * @authors Markus Lång
 * @brief Random number generation. Set of functions used for ray bounce direction
 * @version 0.1
 * @date 2023-11-08
 */


#ifndef RAND_H
#define RAND_H


#include <random>
#include <iostream>
#include "vector.hpp"

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
