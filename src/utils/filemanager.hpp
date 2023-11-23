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
    bool writeOutput(const std::string& outputPath, const std::vector<std::vector<Colour>> &outputBuffer, unsigned int xDim, unsigned int yDim);

    /**
     * Saves an image using SFML method
     * @param outputPath
     * @param image
     * @return bool
     */
    bool saveRenderImage(const std::string& outputPath, sf::Image &image);

    /**
     * Reads sf::Color from json format
     * @param json reference
     * @param name of the attribute
     * @return sf::Color
     */
    auto ColorFromJSON(const json& j, const std::string& name);

    /**
     * Reads Vector from json format
     * @param json reference
     * @param name of the attribute
     * @return Vector
     */
    auto VectorFromJSON(const json& j, const std::string& name);

    /**
     * Reads Camera from json format
     * @param json reference
     * @return Camera
     */
    Camera CameraFromJSON(const json& j);

    /**
     * Reads Material from json format
     * @param json reference
     * @return Material
     */
    auto MaterialFromJSON(const json& j);

    /**
     * Reads Sphere from json format
     * @param json reference
     * @param mat Material reference
     * @return Sphere
     */
    auto SphereFromJSON(const json& j, const Material& mat);

    /**
     * Creates a scene based on a json file
     * @param inputPath
     * @return Scene
     */
    Scene createScene(const std::string &inputPath);
}

#endif // FILEMANAGER_HPP