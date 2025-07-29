#include "core/Engine.hpp"
#include "core/IModule.hpp"
#include "core/ServiceLocator.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

namespace GameEngine {

EngineCore* g_engine = nullptr;

EngineCore::EngineCore() 
    : running(false), deltaTime(0.0f), totalTime(0.0f) {
    g_engine = this;
}

EngineCore::~EngineCore() {
    Shutdown();
    g_engine = nullptr;
}

bool EngineCore::Initialize(const std::string& configPath) {
    std::cout << "Initializing Game Engine..." << std::endl;
    
    // Load configuration
    if (!configPath.empty()) {
        LoadConfiguration(configPath);
    }
    
    // Initialize modules
    for (auto& module : modules) {
        if (!module->Initialize(*this)) {
            std::cerr << "Failed to initialize module: " << module->GetName() << std::endl;
            return false;
        }
        std::cout << "Initialized module: " << module->GetName() << std::endl;
    }
    
    // Initialize timing
    lastFrameTime = std::chrono::high_resolution_clock::now();
    
    std::cout << "Game Engine initialized successfully!" << std::endl;
    return true;
}

void EngineCore::Shutdown() {
    std::cout << "Shutting down Game Engine..." << std::endl;
    
    running = false;
    
    // Shutdown modules in reverse order
    for (auto it = modules.rbegin(); it != modules.rend(); ++it) {
        (*it)->Shutdown();
        std::cout << "Shutdown module: " << (*it)->GetName() << std::endl;
    }
    
    modules.clear();
    std::cout << "Game Engine shutdown complete!" << std::endl;
}

void EngineCore::Run() {
    if (running) {
        std::cout << "Engine is already running!" << std::endl;
        return;
    }
    
    running = true;
    std::cout << "Starting Game Engine main loop..." << std::endl;
    
    while (running) {
        Update();
        Render();
        
        // Frame rate limiting (60 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void EngineCore::Stop() {
    running = false;
    std::cout << "Stopping Game Engine..." << std::endl;
}

bool EngineCore::LoadModule(const std::string& modulePath) {
    // TODO: Implement dynamic module loading
    std::cout << "Loading module from: " << modulePath << std::endl;
    return true;
}

void EngineCore::UnloadModule(const std::string& moduleName) {
    // TODO: Implement dynamic module unloading
    std::cout << "Unloading module: " << moduleName << std::endl;
}

void EngineCore::ReloadModule(const std::string& moduleName) {
    std::cout << "Reloading module: " << moduleName << std::endl;
    
    // Find the module
    for (auto& module : modules) {
        if (moduleName == module->GetName()) {
            module->OnReload();
            std::cout << "Module reloaded: " << moduleName << std::endl;
            return;
        }
    }
    
    std::cerr << "Module not found: " << moduleName << std::endl;
}

void EngineCore::LoadConfiguration(const std::string& configPath) {
    std::cout << "Loading configuration from: " << configPath << std::endl;
    
    try {
        std::ifstream file(configPath);
        if (file.is_open()) {
            file >> config;
            std::cout << "Configuration loaded successfully!" << std::endl;
        } else {
            std::cerr << "Failed to open configuration file: " << configPath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to load configuration: " << e.what() << std::endl;
    }
}

void EngineCore::Update() {
    // Calculate delta time
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFrameTime);
    deltaTime = duration.count() / 1000000.0f;
    lastFrameTime = currentTime;
    
    // Clamp delta time to prevent large jumps
    deltaTime = glm::clamp(deltaTime, 0.0f, 0.1f);
    totalTime += deltaTime;
    
    // Update modules
    for (auto& module : modules) {
        module->Update(deltaTime);
    }
}

void EngineCore::Render() {
    // Render modules
    for (auto& module : modules) {
        module->Render();
    }
}

void EngineCore::ProcessEvents() {
    // TODO: Implement event processing
    // This will handle window events, input events, etc.
}

} // namespace GameEngine