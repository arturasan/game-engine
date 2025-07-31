#pragma once

#include <vector>
#include <memory>

#include "core/IModule.hpp"

class ModuleManager {
public:
    void RegisterModule(std::unique_ptr<IModule> module);
    void Initialize(EngineCore& engine);
    void Shutdown();
    void Update(float deltaTime);
    void Render();

private:
    std::vector<std::unique_ptr<IModule>> modules_;
};
