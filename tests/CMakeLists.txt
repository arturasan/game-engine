# Test suite
add_subdirectory(core)

# Test runner
add_executable(test_runner
    test_main.cpp
)

# Include directories
target_include_directories(test_runner
    PRIVATE ${CMAKE_SOURCE_DIR}/engine/include
    PRIVATE ${CMAKE_SOURCE_DIR}/modules/renderer_interface/include
)

# Link libraries
target_link_libraries(test_runner
    PRIVATE
        engine_core
        renderer_interface
        gtest::gtest
        gtest::gtest_main
)

# Set C++ standard
target_compile_features(test_runner PRIVATE cxx_std_20)

# Set output directory
set_target_properties(test_runner PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)