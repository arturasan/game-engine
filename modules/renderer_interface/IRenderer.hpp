#pragma once

#include <glm/glm.hpp>

// Forward declarations for handles
using TextureHandle = unsigned int;
using ShaderHandle = unsigned int;
using MeshHandle = unsigned int;

struct TextureDesc {};
struct ShaderDesc {};
struct MeshDesc {};

class IRenderer {
public:
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    
    // Resource creation
    virtual TextureHandle CreateTexture(const TextureDesc& desc) = 0;
    virtual ShaderHandle CreateShader(const ShaderDesc& desc) = 0;
    virtual MeshHandle CreateMesh(const MeshDesc& desc) = 0;
    
    // Drawing
    virtual void DrawMesh(MeshHandle mesh, const glm::mat4& transform) = 0;
    virtual void DrawFullscreenQuad(ShaderHandle shader) = 0;
    
    // Debug rendering
    virtual void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) = 0;
};
