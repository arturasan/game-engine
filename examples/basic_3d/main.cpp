#include "core/Engine.hpp"
#include "ecs/Components.hpp"
#include "ecs/Systems.hpp"
#include "modules/IRenderer.hpp"
#include <iostream>
#include <memory>
#include <cmath>

using namespace GameEngine;

// Simple 3D game class
class Basic3DGame {
public:
    Basic3DGame() = default;
    ~Basic3DGame() = default;
    
    bool Initialize() {
        std::cout << "Initializing Basic 3D Game..." << std::endl;
        
        // Create engine
        engine = std::make_unique<EngineCore>();
        
        // Initialize engine
        if (!engine->Initialize()) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return false;
        }
        
        // Setup scene
        SetupScene();
        
        std::cout << "Basic 3D Game initialized successfully!" << std::endl;
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
    float time = 0.0f;
    
    void SetupScene() {
        std::cout << "Setting up 3D scene..." << std::endl;
        
        // Get renderer service
        auto& renderer = ServiceLocator<IRenderer>::Get();
        
        // Create a ground plane
        auto ground = registry.create();
        registry.emplace<TransformComponent>(ground,
            glm::vec3{0.0f, -2.0f, 0.0f},
            glm::quat{1.0f, 0.0f, 0.0f, 0.0f},
            glm::vec3{10.0f, 0.1f, 10.0f}
        );
        registry.emplace<RenderableComponent>(ground,
            "assets/plane.mesh",
            "assets/ground.material",
            true,
            0
        );
        registry.emplace<TagComponent>(ground, "Ground");
        
        // Create some rotating cubes
        for (int i = 0; i < 5; i++) {
            auto cube = registry.create();
            float angle = (i / 5.0f) * 2.0f * glm::pi<float>();
            float radius = 3.0f;
            
            registry.emplace<TransformComponent>(cube,
                glm::vec3{glm::cos(angle) * radius, 0.0f, glm::sin(angle) * radius},
                glm::quat{1.0f, 0.0f, 0.0f, 0.0f},
                glm::vec3{0.5f, 0.5f, 0.5f}
            );
            registry.emplace<RenderableComponent>(cube,
                "assets/cube.mesh",
                "assets/cube.material",
                true,
                1
            );
            registry.emplace<TagComponent>(cube, "RotatingCube");
            
            // Store original position for animation
            registry.emplace<struct CubeData>(cube, angle, radius);
        }
        
        // Create a central sphere
        auto sphere = registry.create();
        registry.emplace<TransformComponent>(sphere,
            glm::vec3{0.0f, 0.0f, 0.0f},
            glm::quat{1.0f, 0.0f, 0.0f, 0.0f},
            glm::vec3{0.5f, 0.5f, 0.5f}
        );
        registry.emplace<RenderableComponent>(sphere,
            "assets/sphere.mesh",
            "assets/sphere.material",
            true,
            1
        );
        registry.emplace<TagComponent>(sphere, "CentralSphere");
        
        // Create a camera
        auto camera = registry.create();
        registry.emplace<TransformComponent>(camera,
            glm::vec3{0.0f, 3.0f, 8.0f},
            glm::quat{glm::angleAxis(glm::radians(-20.0f), glm::vec3{1.0f, 0.0f, 0.0f})},
            glm::vec3{1.0f, 1.0f, 1.0f}
        );
        registry.emplace<CameraComponent>(camera,
            CameraComponent::ProjectionType::Perspective,
            45.0f,    // fov
            0.1f,     // near plane
            100.0f,   // far plane
            10.0f     // ortho size (not used for perspective)
        );
        registry.emplace<TagComponent>(camera, "MainCamera");
        
        // Create some lights
        auto directionalLight = registry.create();
        registry.emplace<TransformComponent>(directionalLight,
            glm::vec3{5.0f, 10.0f, 5.0f},
            glm::quat{glm::angleAxis(glm::radians(-45.0f), glm::vec3{1.0f, 0.0f, 0.0f})},
            glm::vec3{1.0f, 1.0f, 1.0f}
        );
        registry.emplace<LightComponent>(directionalLight,
            LightComponent::Type::Directional,
            glm::vec3{1.0f, 1.0f, 1.0f},
            0.8f,
            10.0f,
            45.0f,
            30.0f,
            true,
            0.005f,
            1024
        );
        registry.emplace<TagComponent>(directionalLight, "DirectionalLight");
        
        // Add a point light
        auto pointLight = registry.create();
        registry.emplace<TransformComponent>(pointLight,
            glm::vec3{0.0f, 2.0f, 0.0f},
            glm::quat{1.0f, 0.0f, 0.0f, 0.0f},
            glm::vec3{1.0f, 1.0f, 1.0f}
        );
        registry.emplace<LightComponent>(pointLight,
            LightComponent::Type::Point,
            glm::vec3{1.0f, 0.5f, 0.2f},
            1.0f,
            5.0f,
            45.0f,
            30.0f,
            false,
            0.005f,
            512
        );
        registry.emplace<TagComponent>(pointLight, "PointLight");
        
        std::cout << "3D scene setup complete!" << std::endl;
    }
    
    void Update(float deltaTime) {
        time += deltaTime;
        
        // Animate rotating cubes
        auto view = registry.view<TransformComponent, TagComponent, CubeData>();
        view.each([this](auto& transform, auto& tag, auto& cubeData) {
            if (tag.tag == "RotatingCube") {
                // Rotate around Y axis
                float angle = cubeData.originalAngle + time * 0.5f;
                transform.position.x = glm::cos(angle) * cubeData.radius;
                transform.position.z = glm::sin(angle) * cubeData.radius;
                
                // Rotate the cube itself
                transform.rotation = glm::angleAxis(time, glm::vec3{0.0f, 1.0f, 0.0f});
            }
        });
        
        // Animate central sphere (pulsing)
        auto sphereView = registry.view<TransformComponent, TagComponent>();
        sphereView.each([this](auto& transform, auto& tag) {
            if (tag.tag == "CentralSphere") {
                float scale = 0.5f + 0.2f * glm::sin(time * 2.0f);
                transform.scale = glm::vec3{scale, scale, scale};
            }
        });
        
        // Animate point light
        auto lightView = registry.view<TransformComponent, TagComponent>();
        lightView.each([this](auto& transform, auto& tag) {
            if (tag.tag == "PointLight") {
                transform.position.x = 2.0f * glm::sin(time);
                transform.position.z = 2.0f * glm::cos(time);
            }
        });
    }
    
    // Component for cube animation data
    struct CubeData {
        float originalAngle;
        float radius;
        
        CubeData(float angle, float r) : originalAngle(angle), radius(r) {}
    };
};

int main(int argc, char* argv[]) {
    std::cout << "=== Basic 3D Game Example ===" << std::endl;
    
    Basic3DGame game;
    
    if (!game.Initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }
    
    game.Run();
    game.Shutdown();
    
    std::cout << "Game exited successfully!" << std::endl;
    return 0;
}