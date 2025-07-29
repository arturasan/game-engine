#include "core/Engine.hpp"
#include "ecs/Components.hpp"
#include "ecs/Systems.hpp"
#include "modules/IRenderer.hpp"
#include <iostream>
#include <memory>

using namespace GameEngine;

// Simple game class
class Basic2DGame {
public:
    Basic2DGame() = default;
    ~Basic2DGame() = default;
    
    bool Initialize() {
        std::cout << "Initializing Basic 2D Game..." << std::endl;
        
        // Create engine
        engine = std::make_unique<EngineCore>();
        
        // Initialize engine
        if (!engine->Initialize()) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return false;
        }
        
        // Setup scene
        SetupScene();
        
        std::cout << "Basic 2D Game initialized successfully!" << std::endl;
        return true;
    }
    
    void Run() {
        if (engine) {
            engine->Run();
        }
    }
    
    void Shutdown() {
        if (engine) {
            engine->Shutdown();
            engine.reset();
        }
    }
    
private:
    std::unique_ptr<EngineCore> engine;
    entt::registry registry;
    
    void SetupScene() {
        std::cout << "Setting up 2D scene..." << std::endl;
        
        // Create a simple scene with some entities
        
        // Create a player entity
        auto player = registry.create();
        registry.emplace<TransformComponent>(player, 
            glm::vec3{0.0f, 0.0f, 0.0f},  // position
            glm::quat{1.0f, 0.0f, 0.0f, 0.0f},  // rotation
            glm::vec3{1.0f, 1.0f, 1.0f}   // scale
        );
        registry.emplace<RenderableComponent>(player,
            "assets/square.mesh",    // mesh path
            "assets/player.material", // material path
            true,                     // visible
            1                         // layer
        );
        registry.emplace<TagComponent>(player, "Player");
        
        // Create some obstacle entities
        for (int i = 0; i < 5; i++) {
            auto obstacle = registry.create();
            registry.emplace<TransformComponent>(obstacle,
                glm::vec3{static_cast<float>(i * 2 - 4), 3.0f, 0.0f},
                glm::quat{1.0f, 0.0f, 0.0f, 0.0f},
                glm::vec3{0.5f, 0.5f, 0.5f}
            );
            registry.emplace<RenderableComponent>(obstacle,
                "assets/square.mesh",
                "assets/obstacle.material",
                true,
                1
            );
            registry.emplace<TagComponent>(obstacle, "Obstacle");
        }
        
        // Create a camera
        auto camera = registry.create();
        registry.emplace<TransformComponent>(camera,
            glm::vec3{0.0f, 0.0f, 10.0f},
            glm::quat{1.0f, 0.0f, 0.0f, 0.0f},
            glm::vec3{1.0f, 1.0f, 1.0f}
        );
        registry.emplace<CameraComponent>(camera,
            CameraComponent::ProjectionType::Orthographic,
            45.0f,    // fov
            0.1f,     // near plane
            1000.0f,  // far plane
            10.0f     // ortho size
        );
        registry.emplace<TagComponent>(camera, "MainCamera");
        
        std::cout << "Scene setup complete!" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::cout << "=== Basic 2D Game Example ===" << std::endl;
    
    Basic2DGame game;
    
    if (!game.Initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }
    
    game.Run();
    game.Shutdown();
    
    std::cout << "Game exited successfully!" << std::endl;
    return 0;
}