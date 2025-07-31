#pragma once

#include "renderer_interface/IRenderer.hpp"
#include "core/IModule.hpp"

class RaylibRenderer : public IRenderer, public IModule {
public:
    // IModule
    bool Initialize(EngineCore& engine) override;
    void Shutdown() override;
    void Render() override;
    const char* GetName() const override { return "renderer_raylib"; }
    uint32_t GetVersion() const override { return 1; }

    // IRenderer
    bool Initialize() override;
    void BeginFrame() override;
    void EndFrame() override;
    TextureHandle CreateTexture(const TextureDesc& desc) override;
    ShaderHandle CreateShader(const ShaderDesc& desc) override;
    MeshHandle CreateMesh(const MeshDesc& desc) override;
    void DrawMesh(MeshHandle mesh, const glm::mat4& transform) override;
    void DrawFullscreenQuad(ShaderHandle shader) override;
    void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) override;
};
