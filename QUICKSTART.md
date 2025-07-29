# Game Engine Quick Start Guide

This guide will help you get started with the Game Engine quickly.

## Prerequisites

- C++20 compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.20 or higher
- vcpkg for dependency management
- Python 3.8+ for tools

## Installation

### 1. Install Dependencies

```bash
# Install vcpkg (if not already installed)
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

# Install required packages
./vcpkg install entt raylib glm nlohmann-json spdlog gtest
```

### 2. Build the Engine

```bash
# Clone the repository
git clone <repository-url>
cd game-engine

# Build using the provided script
./scripts/build.sh --release

# Or manually:
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

## Basic Usage

### Creating a Simple Game

```cpp
#include "core/Engine.hpp"
#include "ecs/Components.hpp"
#include "modules/IRenderer.hpp"

using namespace GameEngine;

int main() {
    // Create and initialize engine
    EngineCore engine;
    if (!engine.Initialize("config.json")) {
        return 1;
    }
    
    // Get services
    auto& renderer = ServiceLocator<IRenderer>::Get();
    
    // Create game objects
    entt::registry registry;
    
    // Create a player entity
    auto player = registry.create();
    registry.emplace<TransformComponent>(player, glm::vec3{0, 0, 0});
    registry.emplace<RenderableComponent>(player, "player.mesh", "player.material");
    registry.emplace<TagComponent>(player, "Player");
    
    // Create a camera
    auto camera = registry.create();
    registry.emplace<TransformComponent>(camera, glm::vec3{0, 5, 10});
    registry.emplace<CameraComponent>(camera);
    registry.emplace<TagComponent>(camera, "Camera");
    
    // Run the game
    engine.Run();
    
    // Cleanup
    engine.Shutdown();
    return 0;
}
```

### Running Examples

The engine includes several examples to get you started:

```bash
# Run the 2D example
./bin/examples/basic_2d

# Run the 3D example
./bin/examples/basic_3d
```

## Module Development

### Creating a New Module

Use the module generator to create boilerplate code:

```bash
# Generate a new module
python3 tools/module_generator/module_generator.py \
    --name "my_system" \
    --version "1.0.0" \
    --description "My custom system" \
    --dependencies "renderer_interface" \
    --tags "custom","system"
```

This will create:
- `modules/my_system/` - Module directory
- `modules/my_system/include/my_system.hpp` - Header file
- `modules/my_system/src/my_system.cpp` - Implementation
- `modules/my_system/CMakeLists.txt` - Build file
- `modules/my_system/module.json` - Module metadata

### Implementing a Module

```cpp
// my_system.hpp
#pragma once
#include "core/IModule.hpp"

namespace GameEngine {
    class MySystem : public IModule {
    public:
        bool Initialize(EngineCore& engine) override;
        void Shutdown() override;
        void Update(float deltaTime) override;
        const char* GetName() const override { return "my_system"; }
        uint32_t GetVersion() const override { return 0x01000000; }
    };
}
```

```cpp
// my_system.cpp
#include "my_system.hpp"
#include <iostream>

namespace GameEngine {
    bool MySystem::Initialize(EngineCore& engine) {
        this->engine = &engine;
        std::cout << "MySystem initialized!" << std::endl;
        return true;
    }
    
    void MySystem::Shutdown() {
        std::cout << "MySystem shutdown!" << std::endl;
    }
    
    void MySystem::Update(float deltaTime) {
        // Your update logic here
    }
}

// Module factory function
extern "C" IModule* CreateMySystem() {
    return new MySystem();
}
```

## Asset Management

### Cooking Assets

Use the asset cooker to process raw assets:

```bash
# Cook all assets in a directory
python3 tools/asset_cooker/asset_cooker.py assets/ cooked_assets/

# Cook a single file
python3 tools/asset_cooker/asset_cooker.py assets/texture.png cooked_assets/

# Generate asset manifest
python3 tools/asset_cooker/asset_cooker.py assets/ cooked_assets/ --generate-manifest
```

### Supported Asset Types

- **Textures**: PNG, JPG, BMP, TGA
- **Models**: OBJ, GLTF, GLB, FBX
- **Audio**: WAV, MP3, OGG, FLAC
- **Shaders**: .vert, .frag, .geom, .comp

## Configuration

The engine uses JSON configuration files. Here's a basic example:

```json
{
    "engine": {
        "name": "My Game",
        "window": {
            "title": "My Game",
            "width": 1280,
            "height": 720
        }
    },
    "renderer": {
        "backend": "raylib",
        "clear_color": [0.1, 0.1, 0.1, 1.0]
    },
    "assets": {
        "paths": ["assets", "cooked_assets"],
        "hot_reload": true
    }
}
```

## Testing

Run the test suite to verify everything works:

```bash
# Run all tests
./bin/tests/test_runner

# Run specific test suite
./bin/tests/core_tests
```

## Architecture Overview

### Core Components

1. **Engine Core**: Main game loop and module management
2. **ECS**: Entity Component System using EnTT
3. **Service Locator**: Dependency injection pattern
4. **Module System**: Hot-reloadable plugins

### Module System

The engine uses a modular architecture where each system is a separate module:

- **Renderer**: Handles graphics rendering
- **Asset Manager**: Manages resource loading
- **Physics**: Simulates physics
- **Audio**: Handles sound playback
- **Scripting**: Lua integration
- **Editor**: In-game development tools

### Entity Component System

The ECS provides a flexible way to compose game objects:

```cpp
// Create an entity
auto entity = registry.create();

// Add components
registry.emplace<TransformComponent>(entity, position, rotation, scale);
registry.emplace<RenderableComponent>(entity, mesh, material);
registry.emplace<PhysicsComponent>(entity, mass, friction);

// Query entities
auto view = registry.view<TransformComponent, RenderableComponent>();
view.each([](auto& transform, auto& renderable) {
    // Process entity
});
```

## Performance Tips

1. **Use Object Pooling**: For frequently created/destroyed entities
2. **Batch Rendering**: Minimize draw calls
3. **Asset Hot-Reloading**: Enable for faster iteration
4. **Memory Management**: Monitor memory usage with built-in tools

## Debugging

Enable debug features in configuration:

```json
{
    "debug": {
        "enabled": true,
        "show_fps": true,
        "show_memory": true,
        "profiler": true
    }
}
```

## Getting Help

- Check the `documentation/` directory for detailed guides
- Look at the examples in `examples/`
- Review the source code comments
- Run tests to understand expected behavior

## Next Steps

1. **Run the examples** to see the engine in action
2. **Create a simple game** using the provided templates
3. **Develop a custom module** for your specific needs
4. **Contribute** to the engine by fixing bugs or adding features

Happy game development!