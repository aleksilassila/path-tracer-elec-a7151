/**
 * @file main.cpp
 * @authors Markus Lång
 * @brief Tests for rand.h
 * @version 0.1
 * @date 2023-11-08
 */

#include <iostream>
#include "../../src/utils/rand.h"



int main(void){

    std::cout << "Testing rand.h ... \nMaking random scalar vector:" << std::endl;
    Vector randVec = Random::RayScalar(0, 2);
    std::cout << randVec << std::endl;
    return 0;

}




