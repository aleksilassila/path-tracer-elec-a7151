#include "doctest.h"
#include "../src/utils/filemanager.hpp"

TEST_CASE("example") {
    Scene scene = FileManager::createScene("/files/example.json");
}

//todo so far it terminates, but at least complies
TEST_CASE("Testing output writing") {
//    /*
//    * test writing a buffer to file
//    */
//
//    unsigned int yDim = 255;
//    unsigned int xDim = 255;
//
//    std::vector<std::vector<Colour>> buffer(yDim, std::vector<Colour>());
//
//    // Fill the buffer
//    for (unsigned int i = 0; i < yDim; i++) {
//        for (unsigned int j = 0; j < xDim; j++) {
//            buffer[j][i] = {(unsigned char) i, (unsigned char) j, 255};
//        }
//    }
//
//    FileManager fm("input.txt", "output.ppm");
//    bool result = fm.writeOutput(buffer, xDim, yDim);
//    if (result) {
//        std::cout << "Test WriteOutput: Passed" << std::endl;
//    } else {
//        std::cout << "Test WriteOutput: Failed" << std::endl;
//    }
}
