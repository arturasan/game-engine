#pragma once

#include "modules/IRenderer.hpp"
#include "core/IModule.hpp"
#include <raylib.h>
#include <unordered_map>
#include <memory>

namespace GameEngine {

class RaylibRenderer : public IRenderer, public IModule {
public:
    // IModule interface
    bool Initialize(EngineCore& engine) override;
    void Shutdown() override;
    void Update(float deltaTime) override;
    void Render() override;
    const char* GetName() const override { return "renderer_raylib"; }
    uint32_t GetVersion() const override { return 1000; } // 1.0.0
    
    // IRenderer interface
    bool Initialize() override;
    void Shutdown() override;
    void BeginFrame() override;
    void EndFrame() override;
    void Present() override;
    
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void GetViewport(uint32_t& x, uint32_t& y, uint32_t& width, uint32_t& height) const override;
    
    void ClearColor(const glm::vec4& color) override;
    void ClearDepth(float depth) override;
    void ClearStencil(int32_t stencil) override;
    void Clear(bool color, bool depth, bool stencil) override;
    
    TextureHandle CreateTexture(const TextureDesc& desc) override;
    ShaderHandle CreateShader(const ShaderDesc& desc) override;
    MeshHandle CreateMesh(const MeshDesc& desc) override;
    MaterialHandle CreateMaterial(const MaterialDesc& desc) override;
    
    void DestroyTexture(TextureHandle handle) override;
    void DestroyShader(ShaderHandle handle) override;
    void DestroyMesh(MeshHandle handle) override;
    void DestroyMaterial(MaterialHandle handle) override;
    
    void DrawMesh(MeshHandle mesh, MaterialHandle material, const glm::mat4& transform) override;
    void DrawMeshInstanced(MeshHandle mesh, MaterialHandle material, const glm::mat4* transforms, uint32_t count) override;
    void DrawFullscreenQuad(MaterialHandle material) override;
    
    void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) override;
    void DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color) override;
    void DrawSphere(const glm::vec3& center, float radius, const glm::vec3& color) override;
    void DrawGrid(const glm::vec3& center, float size, float step, const glm::vec3& color) override;
    
    void SetRenderState(const RenderState& state) override;
    const RenderState& GetRenderState() const override;
    
    void SetViewMatrix(const glm::mat4& view) override;
    void SetProjectionMatrix(const glm::mat4& projection) override;
    const glm::mat4& GetViewMatrix() const override;
    const glm::mat4& GetProjectionMatrix() const override;
    
    void SetAmbientLight(const glm::vec3& color) override;
    const glm::vec3& GetAmbientLight() const override;
    
    void SetWindowTitle(const std::string& title) override;
    void SetWindowSize(uint32_t width, uint32_t height) override;
    void GetWindowSize(uint32_t& width, uint32_t& height) const override;
    bool ShouldClose() const override;
    
    float GetDeltaTime() const override;
    float GetTotalTime() const override;
    
    void SetVSync(bool enabled) override;
    bool GetVSync() const override;
    
    uint32_t GetDrawCalls() const override;
    uint32_t GetTriangleCount() const override;
    void ResetStatistics() override;

private:
    // Raylib specific resources
    std::unordered_map<TextureHandle, Texture2D> textures;
    std::unordered_map<ShaderHandle, Shader> shaders;
    std::unordered_map<MeshHandle, Mesh> meshes;
    std::unordered_map<MaterialHandle, std::unique_ptr<struct MaterialData>> materials;
    
    // Handle management
    TextureHandle nextTextureHandle = 1;
    ShaderHandle nextShaderHandle = 1;
    MeshHandle nextMeshHandle = 1;
    MaterialHandle nextMaterialHandle = 1;
    
    // State
    RenderState currentRenderState;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 ambientLight{0.2f, 0.2f, 0.2f};
    
    // Statistics
    uint32_t drawCalls = 0;
    uint32_t triangleCount = 0;
    
    // Window properties
    uint32_t viewportX = 0, viewportY = 0;
    uint32_t viewportWidth = 800, viewportHeight = 600;
    
    // Helper functions
    Color ToRaylibColor(const glm::vec3& color);
    Color ToRaylibColor(const glm::vec4& color);
    Vector3 ToRaylibVector(const glm::vec3& vec);
    Matrix ToRaylibMatrix(const glm::mat4& mat);
    
    // Resource management
    void CleanupResources();
};

// Material data structure
struct MaterialData {
    ShaderHandle shader;
    std::unordered_map<std::string, float> floatParams;
    std::unordered_map<std::string, glm::vec3> vec3Params;
    std::unordered_map<std::string, glm::vec4> vec4Params;
    std::unordered_map<std::string, TextureHandle> textureParams;
};

// Module factory function
extern "C" IModule* CreateRaylibRenderer();

} // namespace GameEngine