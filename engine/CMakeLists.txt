# Core engine library
add_library(engine_core STATIC
    src/Engine.cpp
)

# Include directories
target_include_directories(engine_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(engine_core
    PUBLIC
        EnTT::EnTT
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        glm::glm
    PRIVATE
        raylib::raylib
)

# Set C++ standard
target_compile_features(engine_core PUBLIC cxx_std_20)

# Add definitions
target_compile_definitions(engine_core
    PUBLIC
        ENGINE_EXPORTS
    PRIVATE
        SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)

# Set properties
set_target_properties(engine_core PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

# Install
install(TARGETS engine_core
    EXPORT GameEngineTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)