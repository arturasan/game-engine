cmake_minimum_required(VERSION 3.20)
project(GameEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_EXAMPLES "Build example projects" ON)
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_TOOLS "Build standalone tools" ON)
option(ENABLE_AI "Enable AI tooling" ON)

# Dependencies
find_package(raylib CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Core engine
add_subdirectory(engine)

# Modules
add_subdirectory(modules)

# Tools
if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
