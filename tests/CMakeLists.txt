# doctest run for CLion
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE MAIN_SOURCES "${PROJECT_SOURCE_DIR}/src/utils/*.cpp" "${PROJECT_SOURCE_DIR}/src/world/*.cpp")
add_executable(Doctest_tests_run ${MAIN_SOURCES} ${TEST_SOURCES})

#message(WARNING ${TEST_SOURCES})

