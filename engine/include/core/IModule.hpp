#pragma once

#include <string>

namespace GameEngine {

class EngineCore;

class IModule {
public:
    virtual ~IModule() = default;
    
    // Module lifecycle
    virtual bool Initialize(EngineCore& engine) = 0;
    virtual void Shutdown() = 0;
    
    // Hot-reload support
    virtual void OnReload() {}
    
    // Update and render
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
    
    // Module information
    virtual const char* GetName() const = 0;
    virtual uint32_t GetVersion() const = 0;
    virtual const char* GetDescription() const { return ""; }
    
    // Dependencies
    virtual const std::vector<std::string>& GetDependencies() const {
        static std::vector<std::string> empty;
        return empty;
    }
    
    // Module state
    virtual bool IsInitialized() const { return initialized; }
    
protected:
    bool initialized = false;
    EngineCore* engine = nullptr;
};

// Module entry point function type
typedef IModule* (*CreateModuleFunc)();
typedef void (*DestroyModuleFunc)(IModule*);

} // namespace GameEngine