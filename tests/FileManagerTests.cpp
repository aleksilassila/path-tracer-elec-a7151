#include "doctest.h"
#include "../src/utils/filemanager.hpp"

TEST_CASE("example") {
    Scene scene = FileManager::CreateScene("/files/example.json");
}

TEST_CASE("scene1") {
    Scene scene = FileManager::CreateScene("/files/scene1.json");
}

TEST_CASE("scene2") {
    Scene scene = FileManager::CreateScene("/files/scene2.json");
}

TEST_CASE("scene3") {
    Scene scene = FileManager::CreateScene("/files/scene3.json");
}

TEST_CASE("scene4") {
    Scene scene = FileManager::CreateScene("/files/scene4.json");
}

TEST_CASE("scene5") {
    Scene scene = FileManager::CreateScene("/files/scene5.json");
}



