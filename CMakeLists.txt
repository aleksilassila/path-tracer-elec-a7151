cmake_minimum_required(VERSION 3.10)

project(PathTracer VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)

# SFML submodule
add_subdirectory(libs/SFML)
include_directories(${SFML_INCLUDE_DIR})

# build files
file(GLOB MAIN "src/main.cpp")

file(GLOB UTILS_CPP "src/utils/*.cpp")
file(GLOB UTILS_HPP "src/utils/*.hpp")
file(GLOB UTILS_H "src/utils/*.h")

file(GLOB WORLD_CPP "src/world/*.cpp")
file(GLOB WORLD_HPP "src/world/*.hpp")
file(GLOB WORLD_H "src/world/*.h")

set(SOURCES ${UTILS_CPP} ${UTILS_HPP} ${UTILS_H} ${WORLD_CPP} ${WORLD_HPP} ${WORLD_H})

# test files
# file(GLOB TESTS

#         # "tests/filemanager_tests/test_write_output.cpp"
#         # Other test directories
# )

# App
add_executable(app ${MAIN} ${SOURCES}

        src/world/scene.h
        src/world/camera.h
        src/world/scene.cpp
        src/world/objects.cpp
        src/pathtracer.cpp
)

target_link_libraries(app sfml-graphics sfml-window sfml-system)

# Tests
# add_executable(tests ${TESTS} ${SOURCES})

target_compile_options(app PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
# target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)