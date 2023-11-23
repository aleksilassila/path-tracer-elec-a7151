cmake_minimum_required(VERSION 3.10)

project(PathTracer VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)

add_compile_definitions("MY_FILE_PATH=\"${PROJECT_SOURCE_DIR}\"")

# SFML
set(SFML sfml-graphics sfml-window sfml-system)
add_subdirectory(libs/SFML)
include_directories(${SFML_INCLUDE_DIR})
link_libraries(${SFML})

# Doctest
add_subdirectory(tests)

# JSON
include_directories(libs/JSON)

# app
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(app ${SOURCES})
target_compile_options(app PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)



