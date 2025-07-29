#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

// This is a forward declaration of the main IRenderer interface
// The actual implementation is in engine/include/modules/IRenderer.hpp

namespace GameEngine {

// Forward declare the main renderer interface
class IRenderer;

// Renderer factory interface
class IRendererFactory {
public:
    virtual ~IRendererFactory() = default;
    virtual std::unique_ptr<IRenderer> CreateRenderer() = 0;
    virtual const char* GetName() const = 0;
    virtual uint32_t GetVersion() const = 0;
};

// Register renderer factory function
typedef IRendererFactory* (*CreateRendererFactoryFunc)();

} // namespace GameEngine