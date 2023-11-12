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
    

    double GetRandomDoubleUniform(double min, double max, unsigned int seed){
        /**
         * @brief Random double with uniform distribution
         * 
         * @return std::default_random_engine 
         */
        static std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> distribution(min, max);
        return  distribution(generator);
    }

    double GetRandomDoubleNormal(double stddev, double average, unsigned int seed) {
        /**
         * @brief Random double with normal distribution
         * 
         * @return std::default_random_engine 
         */
        static std::default_random_engine generator(seed);
        std::normal_distribution<double> distribution(average, stddev);
        return distribution(generator);
    }


} // namespace Random