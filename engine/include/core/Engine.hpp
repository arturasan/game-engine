#pragma once

#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include "IModule.hpp"
#include "ServiceLocator.hpp"

namespace GameEngine {

class EngineCore {
public:
    EngineCore();
    ~EngineCore();

    // Engine lifecycle
    bool Initialize(const std::string& configPath = "");
    void Shutdown();
    void Run();
    void Stop();

    // Module management
    bool LoadModule(const std::string& modulePath);
    void UnloadModule(const std::string& moduleName);
    void ReloadModule(const std::string& moduleName);
    
    // Module iteration
    template<typename Func>
    void ForEachModule(Func func) {
        for (auto& module : modules) {
            func(*module);
        }
    }

    // Engine state
    bool IsRunning() const { return running; }
    float GetDeltaTime() const { return deltaTime; }
    float GetTotalTime() const { return totalTime; }
    
    // Configuration
    void LoadConfiguration(const std::string& configPath);
    const nlohmann::json& GetConfig() const { return config; }

private:
    bool running;
    float deltaTime;
    float totalTime;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    
    std::vector<std::unique_ptr<IModule>> modules;
    nlohmann::json config;
    
    // Internal methods
    void Update();
    void Render();
    void ProcessEvents();
};

// Global engine instance
extern EngineCore* g_engine;

} // namespace GameEngine