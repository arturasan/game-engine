#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

namespace GameEngine {

// Forward declarations
class ITexture;
class IShader;
class IMesh;
class IMaterial;

// Handle types for renderer resources
using TextureHandle = uint32_t;
using ShaderHandle = uint32_t;
using MeshHandle = uint32_t;
using MaterialHandle = uint32_t;

// Resource descriptors
struct TextureDesc {
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint8_t* data;
    bool generateMipmaps = true;
    bool sRGB = true;
    enum class Filter {
        Nearest,
        Linear,
        LinearMipmapLinear
    } filter = Filter::LinearMipmapLinear;
    enum class Wrap {
        Repeat,
        Clamp,
        Mirror
    } wrap = Wrap::Repeat;
};

struct ShaderDesc {
    std::string vertexSource;
    std::string fragmentSource;
    std::vector<std::string> defines;
};

struct MeshDesc {
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<float> tangents;
    std::vector<float> bitangents;
};

struct MaterialDesc {
    ShaderHandle shader;
    std::vector<std::pair<std::string, float>> floatParams;
    std::vector<std::pair<std::string, glm::vec3>> vec3Params;
    std::vector<std::pair<std::string, glm::vec4>> vec4Params;
    std::vector<std::pair<std::string, TextureHandle>> textureParams;
};

// Render state
struct RenderState {
    bool depthTest = true;
    bool depthWrite = true;
    bool cullFace = true;
    bool blend = false;
    enum class CullFace {
        Front,
        Back,
        FrontAndBack
    } cullFaceMode = CullFace::Back;
    enum class BlendFunc {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha
    } blendSrc = BlendFunc::SrcAlpha, blendDst = BlendFunc::OneMinusSrcAlpha;
};

// Main renderer interface
class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    // Renderer lifecycle
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    
    // Frame management
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Present() = 0;
    
    // Viewport management
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void GetViewport(uint32_t& x, uint32_t& y, uint32_t& width, uint32_t& height) const = 0;
    
    // Clear operations
    virtual void ClearColor(const glm::vec4& color) = 0;
    virtual void ClearDepth(float depth = 1.0f) = 0;
    virtual void ClearStencil(int32_t stencil = 0) = 0;
    virtual void Clear(bool color = true, bool depth = true, bool stencil = false) = 0;
    
    // Resource creation
    virtual TextureHandle CreateTexture(const TextureDesc& desc) = 0;
    virtual ShaderHandle CreateShader(const ShaderDesc& desc) = 0;
    virtual MeshHandle CreateMesh(const MeshDesc& desc) = 0;
    virtual MaterialHandle CreateMaterial(const MaterialDesc& desc) = 0;
    
    // Resource destruction
    virtual void DestroyTexture(TextureHandle handle) = 0;
    virtual void DestroyShader(ShaderHandle handle) = 0;
    virtual void DestroyMesh(MeshHandle handle) = 0;
    virtual void DestroyMaterial(MaterialHandle handle) = 0;
    
    // Drawing operations
    virtual void DrawMesh(MeshHandle mesh, MaterialHandle material, const glm::mat4& transform) = 0;
    virtual void DrawMeshInstanced(MeshHandle mesh, MaterialHandle material, const glm::mat4* transforms, uint32_t count) = 0;
    virtual void DrawFullscreenQuad(MaterialHandle material) = 0;
    
    // Debug rendering
    virtual void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) = 0;
    virtual void DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color) = 0;
    virtual void DrawSphere(const glm::vec3& center, float radius, const glm::vec3& color) = 0;
    virtual void DrawGrid(const glm::vec3& center, float size, float step, const glm::vec3& color) = 0;
    
    // Render state management
    virtual void SetRenderState(const RenderState& state) = 0;
    virtual const RenderState& GetRenderState() const = 0;
    
    // Camera management
    virtual void SetViewMatrix(const glm::mat4& view) = 0;
    virtual void SetProjectionMatrix(const glm::mat4& projection) = 0;
    virtual const glm::mat4& GetViewMatrix() const = 0;
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
    
    // Lighting
    virtual void SetAmbientLight(const glm::vec3& color) = 0;
    virtual const glm::vec3& GetAmbientLight() const = 0;
    
    // Window management
    virtual void SetWindowTitle(const std::string& title) = 0;
    virtual void SetWindowSize(uint32_t width, uint32_t height) = 0;
    virtual void GetWindowSize(uint32_t& width, uint32_t& height) const = 0;
    virtual bool ShouldClose() const = 0;
    
    // Frame timing
    virtual float GetDeltaTime() const = 0;
    virtual float GetTotalTime() const = 0;
    
    // VSync
    virtual void SetVSync(bool enabled) = 0;
    virtual bool GetVSync() const = 0;
    
    // Statistics
    virtual uint32_t GetDrawCalls() const = 0;
    virtual uint32_t GetTriangleCount() const = 0;
    virtual void ResetStatistics() = 0;
};

} // namespace GameEngine