#include "doctest.h"
#include "../src/utils/filemanager.hpp"

TEST_CASE("example") {
    Scene scene = FileManager::CreateScene("/files/example.json");
}

TEST_CASE("scene1") {
    Scene scene = FileManager::CreateScene("/files/scene1.json");
}

