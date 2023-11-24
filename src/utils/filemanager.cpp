#include <fstream>
#include "filemanager.hpp"

namespace FileManager {

    bool writeOutput(const std::string& outputPath, const std::vector<std::vector<Colour>> &outputBuffer, unsigned int xDim, unsigned int yDim) {
        std::ofstream ofs(outputPath, std::ios::out | std::ios::binary);

        if (!ofs) {
            std::cerr << "Failed to open output file: " << outputPath << std::endl;
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

    bool saveRenderImage(const std::string& outputPath, sf::Image &image) {
        return image.saveToFile(outputPath);
    }

    auto ColorFromJSON(const json& j, const std::string& name) {
        auto vec = j[name].get<std::vector<int>>();
        return sf::Color(vec[0], vec[1], vec[2]);
    }

    auto VectorFromJSON(const json& j, const std::string& name) {
        auto vec = j[name].get<std::vector<int>>();
        return Vector(vec[0], vec[1], vec[2]);
    }

    Camera CameraFromJSON(const json& j) {
        Camera camera(
                VectorFromJSON(j, "position"),
                j.at("viewPlaneDistance"),
                j.at("yaw"),
                j.at("pitch")
        );
        camera.LookAt(VectorFromJSON(j, "target"));
        camera.SetFovDeg(j.at("fov"));
        return camera;
    }

    /*
     * type:
     * - r regular no optional attributes
     * - s specular [specularColor (vec)]
     * - e emissive [specularColor (vec)], [emission (vec)]
     */

    // ugly switch, gotta refactor Material
    // using exact attributes name, they should be standardised (color/colour)
    auto MaterialFromJSON(const json& j) {
        if (j.at("type") == "r") {
            return Material(
                    ColorFromJSON(j, "colour"),
                    j.at("roughness"),
                    j.at("specularIntensity"),
                    j.at("transparency")
            );
        }
        if (j.at("type") == "s") {
            return Material(
                    ColorFromJSON(j, "colour"),
                    j.at("roughness"),
                    j.at("specularIntensity"),
                    j.at("transparency"),
                    ColorFromJSON(j, "specularColor")
            );
        }
        if (j.at("type") == "e") {
            return Material(
                    ColorFromJSON(j, "colour"),
                    j.at("roughness"),
                    j.at("specularIntensity"),
                    j.at("transparency"),
                    ColorFromJSON(j, "specularColor"),
                    VectorFromJSON(j, "emission")
            );
        }
        return Material();
    }

    auto SphereFromJSON(const json& j, const Material& mat) {
        return Object::Sphere(
                VectorFromJSON(j, "origin"),
                j.at("radius"),
                mat
        );
    }

    //gotta add try catch for failed or invalid input file
    Scene createScene(const std::string &inputPath) {
        std::fstream file;
        std::string filePath = MY_FILE_PATH + std::string(inputPath);
        file.open(filePath,std::ios::in);
        std::cout << file.is_open() << " " << file.fail();

        json Data = json::parse(file);

        Camera camera = CameraFromJSON(Data.at("camera"));
        Scene scene = Scene(camera, {});

        std::vector<Material> materials;
        for (size_t i = 0; i < Data.at("materials").size(); i++) {
            materials.push_back(MaterialFromJSON(Data.at("materials")[i]));
        }

        for (size_t i = 0; i < Data.at("spheres").size(); i++) {
            size_t materialIndex = Data.at("spheres")[i].at("materialIndex");
            scene.AddObject(std::make_shared<Object::Sphere>(SphereFromJSON(Data.at("spheres")[i], materials[materialIndex])));
        }
        return scene;
    }

}