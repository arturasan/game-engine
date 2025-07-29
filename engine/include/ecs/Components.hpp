#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

namespace GameEngine {

// Transform component for position, rotation, and scale
struct TransformComponent {
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    
    glm::mat4 GetMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model *= glm::mat4_cast(rotation);
        model = glm::scale(model, scale);
        return model;
    }
    
    void SetEulerAngles(const glm::vec3& angles) {
        rotation = glm::quat(glm::radians(angles));
    }
    
    glm::vec3 GetEulerAngles() const {
        return glm::degrees(glm::eulerAngles(rotation));
    }
};

// Renderable component for entities that can be rendered
struct RenderableComponent {
    std::string meshPath;
    std::string materialPath;
    bool visible = true;
    uint32_t layer = 0;
    
    // Runtime handles (filled by renderer)
    void* meshHandle = nullptr;
    void* materialHandle = nullptr;
};

// Camera component for view/projection matrices
struct CameraComponent {
    enum class ProjectionType {
        Perspective,
        Orthographic
    };
    
    ProjectionType projectionType = ProjectionType::Perspective;
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float orthoSize = 10.0f;
    
    glm::mat4 GetViewMatrix(const TransformComponent& transform) const {
        return glm::inverse(transform.GetMatrix());
    }
    
    glm::mat4 GetProjectionMatrix(float aspectRatio) const {
        if (projectionType == ProjectionType::Perspective) {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        } else {
            float halfSize = orthoSize * 0.5f;
            return glm::ortho(-halfSize * aspectRatio, halfSize * aspectRatio, 
                            -halfSize, halfSize, nearPlane, farPlane);
        }
    }
};

// Light component for lighting
struct LightComponent {
    enum class Type {
        Directional,
        Point,
        Spot
    };
    
    Type type = Type::Directional;
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;
    
    // Point/spot light properties
    float range = 10.0f;
    float spotAngle = 45.0f;
    float innerSpotAngle = 30.0f;
    
    // Shadow properties
    bool castsShadows = false;
    float shadowBias = 0.005f;
    uint32_t shadowMapSize = 1024;
};

// Physics component for collision and dynamics
struct PhysicsComponent {
    enum class BodyType {
        Static,
        Dynamic,
        Kinematic
    };
    
    BodyType bodyType = BodyType::Static;
    float mass = 1.0f;
    float friction = 0.5f;
    float restitution = 0.5f;
    bool useGravity = true;
    
    // Runtime physics body handle
    void* bodyHandle = nullptr;
};

// Script component for custom behavior
struct ScriptComponent {
    std::string scriptPath;
    bool enabled = true;
    
    // Script state
    void* scriptState = nullptr;
};

// Audio component for sound playback
struct AudioComponent {
    std::string audioPath;
    bool looping = false;
    bool playing = false;
    float volume = 1.0f;
    float pitch = 1.0f;
    bool spatial = true;
    float minDistance = 1.0f;
    float maxDistance = 100.0f;
    
    // Runtime audio handle
    void* audioHandle = nullptr;
};

// Tag component for entity categorization
struct TagComponent {
    std::string tag;
    
    TagComponent() = default;
    TagComponent(const std::string& t) : tag(t) {}
};

// Hierarchy component for parent-child relationships
struct HierarchyComponent {
    entt::entity parent = entt::null;
    std::vector<entt::entity> children;
    
    HierarchyComponent() = default;
    HierarchyComponent(entt::entity p) : parent(p) {}
};

} // namespace GameEngine