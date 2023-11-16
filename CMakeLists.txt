cmake_minimum_required(VERSION 3.10)

project(PathTracer VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)

# SFML submodule
add_subdirectory(libs/SFML)
include_directories(${SFML_INCLUDE_DIR})


# App
add_executable(app 
        
        # All source files
        src/main.cpp
        src/utils/material.cpp
        src/world/scene.cpp
        src/world/objects.cpp
        src/pathtracer.cpp
        src/utils/filemanager.cpp
        src/utils/rand.cpp

        )

target_link_libraries(app sfml-graphics sfml-window sfml-system)

target_compile_options(app PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
# target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)