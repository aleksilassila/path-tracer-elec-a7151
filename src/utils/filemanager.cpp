#include <fstream>
#include <unordered_map>

#include "filemanager.hpp"
#include "materialbuilder.hpp"


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
        if (j.contains("color")) builder.setColor(ColorFromJSON(j, "color"));
        if (j.contains("roughness")) builder.setRoughness(j.at("roughness"));
        if (j.contains("specularIntensity")) builder.setSpecularIntensity(j.at("specularIntensity"));
        if (j.contains("specularColor")) builder.setSpecularColor(ColorFromJSON(j, "specularColor"));
        if (j.contains("emission")) builder.setEmission(VectorFromJSON(j, "emission"));
        if (j.contains("n")) builder.setn(j.at("n"));
        if (j.contains("name")) builder.setName(j.at("name"));

        return builder.buildMaterial();
    }

    /**
     * Reads Sphere from json format
     * @param json reference
     * @param mat Material reference
     * @return Sphere
     */
    auto SphereFromJSON(const json& j, const Material& mat) {
        return Object::Sphere(
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
        return Object::Triangle(
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
        return Object::Parallelogram(
                VectorFromJSON(j, "origin"),
                VectorFromJSON(j, "a"),
                VectorFromJSON(j, "b"),
                mat
        );
    }

    // todo add try catch for failed or invalid input files
    Scene createScene(const std::string &inputPath) {
        std::string filePath = MY_FILE_PATH + std::string(inputPath);
        std::fstream file;
        file.open(filePath,std::ios::in);

        std::cout << "File open: " << file.is_open() << " failed: " << file.fail() << std::endl;

        json Data = json::parse(file);

        Camera camera = CameraFromJSON(Data.at("camera"));
        Scene scene = Scene(camera, {});

        std::unordered_map<std::string, Material> materials;
        for (size_t i = 0; i < Data.at("materials").size(); i++) {
            materials[Data.at("materials")[i].at("name")] = MaterialFromJSON(Data.at("materials")[i]);
        }

        for (size_t i = 0; i < Data.at("spheres").size(); i++) {
            std::string materialName = Data.at("spheres")[i].at("materialName");
            scene.AddObject(std::make_shared<Object::Sphere>(SphereFromJSON(Data.at("spheres")[i], materials[materialName])));
        }

        for (size_t i = 0; i < Data.at("triangles").size(); i++) {
            std::string materialName = Data.at("triangles")[i].at("materialName");
            scene.AddObject(std::make_shared<Object::Triangle>(TriangleFromJSON(Data.at("triangles")[i], materials[materialName])));
        }

        for (size_t i = 0; i < Data.at("parallelograms").size(); i++) {
            std::string materialName = Data.at("parallelograms")[i].at("materialName");
            scene.AddObject(std::make_shared<Object::Parallelogram>(ParallelogramFromJSON(Data.at("parallelograms")[i], materials[materialName])));
        }

        return scene;
    }

}