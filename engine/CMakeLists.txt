add_library(engine_core STATIC
    src/Engine.cpp
    src/ModuleManager.cpp
    src/ServiceLocator.cpp
)

target_include_directories(engine_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(engine_core
    PUBLIC
        raylib
)

