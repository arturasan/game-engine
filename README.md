# Game Engine

A modular, high-performance game engine built with C++20, featuring a component-based architecture and hot-reloadable modules.

## Features

- **Modular Architecture**: Hot-reloadable modules for rendering, physics, audio, and more
- **Entity Component System**: Built on EnTT for flexible game object composition
- **Multiple Rendering Backends**: Raylib (current), Vulkan (planned)
- **Physics Integration**: Box2D (2D) and Bullet Physics (3D) support
- **Audio System**: Cross-platform audio with miniaudio
- **Scripting**: Lua integration with sol2 bindings
- **Editor**: Built-in editor with Dear ImGui
- **AI Tooling**: AI-assisted development and optimization

## Architecture

### Core Systems

- **Engine Core**: Main game loop and module management
- **Service Locator**: Dependency injection pattern
- **ECS**: Entity Component System for game objects
- **Module System**: Hot-reloadable plugin architecture

### Modules

- **Renderer**: Abstract rendering interface with multiple implementations
- **Asset Manager**: Resource loading and management
- **Physics**: 2D and 3D physics simulation
- **Audio**: Sound and music playback
- **Scripting**: Lua integration for game logic
- **Editor**: In-game editor and tools

## Building

### Prerequisites

- CMake 3.20 or higher
- C++20 compatible compiler
- vcpkg for dependency management

### Dependencies

Core dependencies managed through vcpkg:
- EnTT (ECS framework)
- Raylib (rendering, input, windowing)
- glm (math library)
- nlohmann/json (JSON parsing)
- spdlog (logging)

### Build Steps

```bash
# Clone the repository
git clone https://github.com/yourusername/game-engine.git
cd game-engine

# Install dependencies
vcpkg install entt raylib glm nlohmann-json spdlog

# Configure build
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build . --config Release
```

## Usage

### Creating a New Game

```cpp
#include "core/Engine.hpp"
#include "modules/IRenderer.hpp"

using namespace GameEngine;

int main() {
    // Create and initialize engine
    EngineCore engine;
    if (!engine.Initialize()) {
        return 1;
    }
    
    // Get renderer service
    auto& renderer = ServiceLocator<IRenderer>::Get();
    
    // Create game objects and systems
    // ...
    
    // Run the game
    engine.Run();
    
    // Cleanup
    engine.Shutdown();
    return 0;
}
```

### Creating a New Module

1. Generate boilerplate:
   ```bash
   ./tools/module_generator.py --name "my_system" --api_version 1
   ```

2. Implement interfaces in `modules/my_system/`

3. Add to `module.json`

4. Build module: `cmake --build . --target my_system`

## Project Structure

```
game-engine/
├── engine/                    # Core engine (static lib)
│   ├── include/
│   │   ├── core/             # Core interfaces
│   │   ├── ecs/              # ECS components & systems
│   │   └── modules/          # Module interfaces
│   └── src/                  # Core engine implementation
│
├── modules/                   # Hot-reloadable modules
│   ├── renderer_interface/   # Abstract renderer interface
│   ├── renderer_raylib/      # Raylib implementation
│   ├── asset_manager/        # Asset pipeline
│   ├── physics_box2d/        # 2D physics
│   └── editor_imgui/         # Editor module
│
├── tools/                     # Standalone tools
│   ├── asset_cooker/         # Asset processing
│   ├── module_generator/     # Module boilerplate generator
│   └── profiler/             # Performance analysis
│
├── examples/                  # Sample projects
│   ├── basic_2d/             # 2D platformer
│   ├── basic_3d/             # 3D scene
│   └── multiplayer_demo/     # Networked game
│
├── tests/                     # Unit & integration tests
└── documentation/             # Engine docs
```

## Examples

### Basic 2D Game

A simple 2D game demonstrating:
- Entity creation and component management
- 2D rendering with Raylib
- Basic input handling
- Simple game loop

Run with:
```bash
./bin/examples/basic_2d
```

### Basic 3D Scene

A 3D scene showcasing:
- 3D model loading and rendering
- Camera management
- Lighting and shadows
- 3D physics integration

Run with:
```bash
./bin/examples/basic_3d
```

## Roadmap

### Phase 1: Core Foundation ✓
- [x] Implement core engine and module system
- [x] Integrate EnTT ECS
- [x] Create Raylib renderer module
- [x] Implement basic asset manager

### Phase 2: Essential Systems
- [ ] Scene management module
- [ ] Physics integration (Box2D)
- [ ] Audio system (miniaudio)
- [ ] Basic editor module

### Phase 3: Tooling
- [ ] Asset cooker implementation
- [ ] Module generator tool
- [ ] Profiler integration
- [ ] AI assistant framework

### Phase 4: Advanced Features
- [ ] Vulkan renderer
- [ ] Scripting system (Lua)
- [ ] Networking support
- [ ] Advanced editor features

### Phase 5: Optimization & Polish
- [ ] Performance optimization
- [ ] Documentation completion
- [ ] Example projects
- [ ] Release preparation

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass
6. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- EnTT for the excellent ECS framework
- Raylib for providing a great starting renderer
- The C++ game development community for inspiration and feedback