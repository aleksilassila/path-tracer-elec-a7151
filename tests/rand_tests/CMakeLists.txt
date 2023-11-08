cmake_minimum_required(VERSION 3.10)
project(RandTests VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)


add_executable(randTests

    ../../src/utils/rand.h
    main.cpp

)

target_compile_options(randTests PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
