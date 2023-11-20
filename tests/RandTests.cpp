#include "doctest.h"
#include "../src/utils/rand.h"

#include <iostream>


TEST_CASE("Testing random output") {
    std::cout << "Testing rand.h ... \nMaking random scalar vectors: \n" << std::endl;
    for (unsigned int i = 0; i < 10; i++){
        double rand = Random::GetRandomDoubleNormal(1, 1, i * i);
        std::cout << rand << std::endl;
    }
}




