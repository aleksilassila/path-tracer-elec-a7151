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

    std::cout << "Testing rand.h ... \nMaking random scalar vectors: \n" << std::endl;
    for (unsigned int i = 0; i < 10; i++){
        double rand = Random::GetRandomDoubleNormal(1, 1, i * i);
        std::cout << rand << std::endl;
    }
    
    return 0;

}




