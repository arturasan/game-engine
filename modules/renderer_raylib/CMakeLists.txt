# Raylib renderer module
add_library(renderer_raylib MODULE
    src/RaylibRenderer.cpp
    include/RaylibRenderer.hpp
)

# Include directories
target_include_directories(renderer_raylib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(renderer_raylib
    PUBLIC
        engine_core
        renderer_interface
    PRIVATE
        raylib::raylib
        glm::glm
        spdlog::spdlog
)

# Set C++ standard
target_compile_features(renderer_raylib PRIVATE cxx_std_20)

# Set properties for module
set_target_properties(renderer_raylib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/modules"
    PREFIX "" # No lib prefix on Unix
    SUFFIX ".module" # Custom suffix for modules
)

# Install
install(TARGETS renderer_raylib
    LIBRARY DESTINATION modules
)