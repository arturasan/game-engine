add_library(renderer_interface INTERFACE)

target_include_directories(renderer_interface
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(renderer_interface INTERFACE glm::glm)
