# Core tests
add_executable(core_tests
    test_engine.cpp
)

# Include directories
target_include_directories(core_tests
    PRIVATE ${CMAKE_SOURCE_DIR}/engine/include
    PRIVATE ${CMAKE_SOURCE_DIR}/modules/renderer_interface/include
)

# Link libraries
target_link_libraries(core_tests
    PRIVATE
        engine_core
        renderer_interface
        gtest::gtest
        gtest::gtest_main
)

# Set C++ standard
target_compile_features(core_tests PRIVATE cxx_std_20)

# Set output directory
set_target_properties(core_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

# Add to test suite
add_test(NAME CoreTests COMMAND core_tests)