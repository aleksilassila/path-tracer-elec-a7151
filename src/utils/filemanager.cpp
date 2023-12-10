#include <fstream>
#include <unordered_map>

#include "filemanager.hpp"
#include "materialbuilder.hpp"
#include "../world/Objects/sphere.hpp"
#include "../world/Objects/triangle.hpp"
#include "../world/Objects/parallelogram.hpp"

namespace FileManager {

    bool WriteOutput(const std::string& outputPath, const std::vector<std::vector<Colour>> &outputBuffer, unsigned int xDim, unsigned int yDim) {
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
                ofs << outputBuffer[i][j].GetRed() << outputBuffer[i][j].GetGreen() << outputBuffer[i][j].GetBlue();
            }
        }
        ofs.close();

        // File written
        return true;
    }

    bool SaveRenderImage(const std::string& outputPath, sf::Image &image) {
        return image.saveToFile(outputPath);
    }

    /**
     * Reads sf::Color from json format
     * @param json reference
     * @param name of the attribute
     * @return sf::Color
     */
    auto ColorFromJSON(const json& j, const std::string& name) {
        auto vec = j[name].get<std::vector<int>>();
        return sf::Color(vec[0], vec[1], vec[2]);
    }

    /**
     * Reads Vector from json format
     * @param json reference
     * @param name of the attribute
     * @return Vector
     */
    auto VectorFromJSON(const json& j, const std::string& name) {
        auto vec = j[name].get<std::vector<int>>();
        return Vector(vec[0], vec[1], vec[2]);
    }

    /**
     * Reads Camera from json format
     * @param json reference
     * @return Camera
     */
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

    /**
     * Reads Material from json format
     * @param json reference
     * @return Material
     */
    auto MaterialFromJSON(const json& j) {
        auto builder = MaterialBuilder();
        if (j.contains("color")) builder.SetColor(ColorFromJSON(j, "color"));
        if (j.contains("roughness")) builder.SetRoughness(j.at("roughness"));
        if (j.contains("specularIntensity")) builder.SetSpecularIntensity(j.at("specularIntensity"));
        if (j.contains("specularColor")) builder.SetSpecularColor(ColorFromJSON(j, "specularColor"));
        if (j.contains("emission")) builder.SetEmission(VectorFromJSON(j, "emission"));
        if (j.contains("n")) builder.SetN(j.at("n"));
        if (j.contains("name")) builder.SetName(j.at("name"));

        return builder.BuildMaterial();
    }

    /**
     * Reads Sphere from json format
     * @param json reference
     * @param mat Material reference
     * @return Sphere
     */
    auto SphereFromJSON(const json& j, const Material& mat) {
        return object::Sphere(
                VectorFromJSON(j, "origin"),
                j.at("radius"),
                mat
        );
    }

    /**
     * Reads Triangle from json format
     * @param json reference
     * @param mat Material reference
     * @return Triangle
     */
    auto TriangleFromJSON(const json& j, const Material& mat) {
        return object::Triangle(
                VectorFromJSON(j, "origin"),
                VectorFromJSON(j, "a"),
                VectorFromJSON(j, "b"),
                mat
        );
    }

    /**
     * Reads Parallelogram from json format
     * @param json reference
     * @param mat Material reference
     * @return Parallelogram
     */
    auto ParallelogramFromJSON(const json& j, const Material& mat) {
        return object::Parallelogram(
                VectorFromJSON(j, "origin"),
                VectorFromJSON(j, "a"),
                VectorFromJSON(j, "b"),
                mat
        );
    }

    Scene CreateScene(const std::string &inputPath) {
        std::string filePath = MY_FILE_PATH + std::string(inputPath);
        std::fstream file;
        try {
            file.open(filePath, std::ios::in);
            json Data = json::parse(file);

            Camera camera = CameraFromJSON(Data.at("camera"));
            Scene scene = Scene(camera, {});

            std::unordered_map<std::string, Material> materials;
            for (size_t i = 0; i < Data.at("materials").size(); i++) {
                materials[Data.at("materials")[i].at("name")] = MaterialFromJSON(Data.at("materials")[i]);
            }

            for (size_t i = 0; i < Data.at("spheres").size(); i++) {
                std::string materialName = Data.at("spheres")[i].at("materialName");
                scene.AddObject(std::make_shared<object::Sphere>(SphereFromJSON(Data.at("spheres")[i], materials[materialName])));
            }

            for (size_t i = 0; i < Data.at("triangles").size(); i++) {
                std::string materialName = Data.at("triangles")[i].at("materialName");
                scene.AddObject(std::make_shared<object::Triangle>(TriangleFromJSON(Data.at("triangles")[i], materials[materialName])));
            }

            for (size_t i = 0; i < Data.at("parallelograms").size(); i++) {
                std::string materialName = Data.at("parallelograms")[i].at("materialName");
                scene.AddObject(std::make_shared<object::Parallelogram>(ParallelogramFromJSON(Data.at("parallelograms")[i], materials[materialName])));
            }
            file.close();
            return scene;
        }
        catch (...)
        {
            std::cerr << "Exception opening or reading file\n";
        }
    }

}