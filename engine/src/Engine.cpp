#include "core/Engine.hpp"
#include "core/ModuleManager.hpp"
#include "raylib.h"

#include <iostream>

EngineCore::EngineCore() {
    module_manager_ = std::make_unique<ModuleManager>();
}

EngineCore::~EngineCore() = default;

void EngineCore::Run() {
    module_manager_->Initialize(*this);

    while(!WindowShouldClose()) {
        // For now, just a simple loop
        // In the future, this will be driven by a renderer module
        // and we'll calculate deltaTime
        module_manager_->Update(0.0f);
        module_manager_->Render();
    }

    module_manager_->Shutdown();
}
