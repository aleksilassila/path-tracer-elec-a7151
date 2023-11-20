#include "filemanager.hpp"


FileManager::FileManager(const std::string &outputPath) : outputPath_(outputPath) {}

FileManager::FileManager(const std::string &inputPath, const std::string &outputPath) : inputPath_(inputPath),
                                                                                        outputPath_(outputPath) {}

bool
FileManager::writeOutput(const std::vector<std::vector<Colour>> &outputBuffer, unsigned int xDim, unsigned int yDim) {

    std::ofstream ofs(outputPath_, std::ios::out | std::ios::binary);

    if (!ofs) {
        std::cerr << "Failed to open output file: " << outputPath_ << std::endl;
        // Failed to write file
        return false;
    }


    // Write PPM file header
    ofs << "P6\n" << xDim << " " << yDim << "\n255\n";

    // write image data
    for (unsigned int i = 0; i < yDim; i++) {
        for (unsigned int j = 0; j < xDim; j++) {
            ofs << outputBuffer[i][j].red() << outputBuffer[i][j].green() << outputBuffer[i][j].blue();
        }
    }
    ofs.close();

    // File written
    return true;
}

void FileManager::saveRenderImage(sf::Image &image) {
    bool result = image.saveToFile(outputPath_);
    std::cout << "Image saved to: " << outputPath_ << std::endl;
}


std::string FileManager::readInput() {

    return "readInput method not implemented";
}