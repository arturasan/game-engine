#include "core/Engine.hpp"
#include "core/ModuleManager.hpp"
#include "modules/renderer_raylib/include/RaylibRenderer.hpp"

#include <memory>

int main() {
    EngineCore engine;
    engine.GetModuleManager().RegisterModule(std::make_unique<RaylibRenderer>());
    engine.Run();
    return 0;
}
