# Basic 2D example
add_executable(basic_2d
    main.cpp
)

# Include directories
target_include_directories(basic_2d
    PRIVATE ${CMAKE_SOURCE_DIR}/engine/include
    PRIVATE ${CMAKE_SOURCE_DIR}/modules/renderer_interface/include
)

# Link libraries
target_link_libraries(basic_2d
    PRIVATE
        engine_core
        renderer_interface
        raylib::raylib
        glm::glm
)

# Set C++ standard
target_compile_features(basic_2d PRIVATE cxx_std_20)

# Set output directory
set_target_properties(basic_2d PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)