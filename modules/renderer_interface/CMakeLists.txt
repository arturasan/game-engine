# Renderer interface module (header-only)
add_library(renderer_interface INTERFACE)

# Include directories
target_include_directories(renderer_interface
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(renderer_interface
    INTERFACE
        engine_core
        glm::glm
)

# Install
install(TARGETS renderer_interface
    EXPORT GameEngineTargets
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)