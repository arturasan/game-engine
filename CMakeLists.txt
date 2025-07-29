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

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Find dependencies
find_package(EnTT REQUIRED)
find_package(raylib REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

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

# Configure package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/GameEngineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(EXPORT GameEngineTargets
    FILE GameEngineTargets.cmake
    DESTINATION lib/cmake/GameEngine
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/GameEngineConfigVersion.cmake"
    DESTINATION lib/cmake/GameEngine
)