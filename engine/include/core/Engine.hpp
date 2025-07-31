#pragma once

#include <memory>

class ModuleManager;

class EngineCore {
public:
    EngineCore();
    ~EngineCore();

    void Run();

    ModuleManager& GetModuleManager() { return *module_manager_; }

private:
    std::unique_ptr<ModuleManager> module_manager_;
};
