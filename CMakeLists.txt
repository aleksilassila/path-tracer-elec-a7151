cmake_minimum_required(VERSION 3.10)

project(PathTracer VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)

# build files
file(GLOB MAIN "src/main.cpp")
file(GLOB SOURCES_C "src/*.c")
file(GLOB SOURCES_CPP "src/*.cpp")
file(GLOB SOURCES_HPP "src/*.hpp")
file(GLOB SOURCES_H "src/*.h")

set(SOURCES ${SOURCES_C} ${SOURCES_CPP} ${SOURCES_H} ${SOURCES_HPP})

# Remove main.cpp from SOURCES
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# test files
file(GLOB TESTS "tests/test_filemanager.cpp")

add_subdirectory(libs/SFML)

include_directories(${SFML_INCLUDE_DIR})

# App
add_executable(app ${MAIN} ${SOURCES}
        src/world/scene.h
        src/world/camera.h
        src/world/scene.cpp
        src/world/objects.cpp)

target_link_libraries(app sfml-graphics sfml-window sfml-system)

# Tests
add_executable(tests ${TESTS} ${SOURCES})

target_compile_options(app PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
