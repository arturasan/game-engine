#include "core/ModuleManager.hpp"

void ModuleManager::RegisterModule(std::unique_ptr<IModule> module) {
    modules_.push_back(std::move(module));
}

void ModuleManager::Initialize(EngineCore& engine) {
    for (auto& module : modules_) {
        module->Initialize(engine);
    }
}

void ModuleManager::Shutdown() {
    for (auto& module : modules_) {
        module->Shutdown();
    }
}

void ModuleManager::Update(float deltaTime) {
    for (auto& module : modules_) {
        module->Update(deltaTime);
    }
}

void ModuleManager::Render() {
    for (auto& module : modules_) {
        module->Render();
    }
}
