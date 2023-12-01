#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <vector>

#include "SFML/Graphics.hpp"
#include "../../libs/JSON/json.hpp"
#include "../world/scene.h"
#include "colour.hpp"

using json = nlohmann::json;

/**
 * Namespace for managing files
 * - Writes output to ppm image file
 * - Saves rendered SFML image
 * - Reads scene from json input file
 */
namespace FileManager {

    /**
     * Writes output buffer to ppm image file
     * @param outputPath
     * @param outputBuffer
     * @param xDim
     * @param yDim
     * @return bool
     */
    bool WriteOutput(const std::string& outputPath, const std::vector<std::vector<Colour>> &outputBuffer, unsigned int xDim, unsigned int yDim);

    /**
     * Saves an image using SFML method
     * @param outputPath
     * @param image
     * @return bool
     */
    bool SaveRenderImage(const std::string& outputPath, sf::Image &image);

    /**
     * Creates a scene based on a json file
     * @param inputPath
     * @return Scene
     */
    Scene CreateScene(const std::string &inputPath);
}

#endif // FILEMANAGER_HPP