add_executable(basic_3d main.cpp)

target_link_libraries(basic_3d PRIVATE engine_core renderer_raylib
    "/home/artand/Projects/game-engine/build/vcpkg_installed/x64-linux/debug/lib/libraylib.a"
    "/home/artand/Projects/game-engine/build/vcpkg_installed/x64-linux/debug/lib/libglfw3.a"
    -lGL -lX11 -lXrandr -lXinerama -lXcursor -lXi -lpthread -ldl -lm
)