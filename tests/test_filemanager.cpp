/*
* Tests for file manager class
*/

#include <iostream>
#include <vector>

#include "../src/filemanager.hpp"
#include "../src/colour.h"


void TestWriteOutput() {
    /*
    * test writing a buffer to file
    */

    unsigned int yDim = 255;
    unsigned int xDim = 255;
   
    std::vector<std::vector<Colour>> buffer(yDim, std::vector<Colour>(xDim));

    // Fill the buffer
    for (unsigned int i = 0; i < yDim; i++) {
        for (unsigned int j = 0; j < xDim; j++) {
            buffer[j][i] = {(unsigned char)i, (unsigned char)j, 255};
        }
    }

    FileManager fm("input.txt", "output.ppm");

    if (fm.writeOutput(buffer, xDim, yDim)) {
        std::cout << "Test WriteOutput: Passed" << std::endl;
    } 
    
    else {
        std::cout << "Test WriteOutput: Failed" << std::endl;
    }
}

int main() {
    // Run tests
    TestWriteOutput();
    return 0;
}