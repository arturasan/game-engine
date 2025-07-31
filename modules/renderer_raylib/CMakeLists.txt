add_library(renderer_raylib STATIC
    src/RaylibRenderer.cpp
)

target_include_directories(renderer_raylib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_SOURCE_DIR}/modules
    PUBLIC ${GLFW3_INCLUDE_DIRS}
)

target_link_libraries(renderer_raylib
    PUBLIC
        engine_core
        renderer_interface
        raylib
        ${GLFW3_LIBRARIES}
)
