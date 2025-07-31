#pragma once

#include <cstdint>

class EngineCore;

class IModule {
public:
    virtual ~IModule() = default;
    virtual bool Initialize(EngineCore& engine) = 0;
    virtual void Shutdown() = 0;
    virtual void OnReload() {} // Hot-reload support
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
    virtual const char* GetName() const = 0;
    virtual uint32_t GetVersion() const = 0;
};
