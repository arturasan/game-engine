#pragma once

#include <entt/entt.hpp>
#include "Components.hpp"

namespace GameEngine {

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void Update(entt::registry& registry, float deltaTime) = 0;
    virtual void Render(entt::registry& registry) {}
    virtual const char* GetName() const = 0;
    virtual bool IsEnabled() const { return enabled; }
    virtual void SetEnabled(bool e) { enabled = e; }
    
protected:
    bool enabled = true;
};

// Transform system for updating hierarchy transformations
class TransformSystem : public ISystem {
public:
    void Update(entt::registry& registry, float deltaTime) override;
    const char* GetName() const override { return "TransformSystem"; }
    
private:
    void UpdateTransformHierarchy(entt::registry& registry, entt::entity entity);
};

// Render system for drawing renderable entities
class RenderSystem : public ISystem {
public:
    void Update(entt::registry& registry, float deltaTime) override;
    void Render(entt::registry& registry) override;
    const char* GetName() const override { return "RenderSystem"; }
};

// Physics system for simulating physics
class PhysicsSystem : public ISystem {
public:
    void Update(entt::registry& registry, float deltaTime) override;
    const char* GetName() const override { return "PhysicsSystem"; }
};

// Script system for executing entity scripts
class ScriptSystem : public ISystem {
public:
    void Update(entt::registry& registry, float deltaTime) override;
    const char* GetName() const override { return "ScriptSystem"; }
};

// Audio system for handling audio playback
class AudioSystem : public ISystem {
public:
    void Update(entt::registry& registry, float deltaTime) override;
    const char* GetName() const override { return "AudioSystem"; }
};

// Light system for managing lighting
class LightSystem : public ISystem {
public:
    void Update(entt::registry& registry, float deltaTime) override;
    void Render(entt::registry& registry) override;
    const char* GetName() const override { return "LightSystem"; }
};

} // namespace GameEngine