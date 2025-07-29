#include "RaylibRenderer.hpp"
#include <iostream>
#include <algorithm>

namespace GameEngine {

// Module factory function
extern "C" IModule* CreateRaylibRenderer() {
    return new RaylibRenderer();
}

bool RaylibRenderer::Initialize(EngineCore& engine) {
    this->engine = &engine;
    std::cout << "Initializing Raylib Renderer module..." << std::endl;
    
    // Initialize the renderer
    if (!Initialize()) {
        std::cerr << "Failed to initialize Raylib renderer!" << std::endl;
        return false;
    }
    
    // Register as a service
    ServiceLocator<IRenderer>::Provide(this);
    
    std::cout << "Raylib Renderer module initialized successfully!" << std::endl;
    return true;
}

void RaylibRenderer::Shutdown() {
    std::cout << "Shutting down Raylib Renderer module..." << std::endl;
    
    // Cleanup resources
    CleanupResources();
    
    // Shutdown Raylib
    CloseWindow();
    
    std::cout << "Raylib Renderer module shutdown complete!" << std::endl;
}

void RaylibRenderer::Update(float deltaTime) {
    // Update renderer-specific logic
}

void RaylibRenderer::Render() {
    // Main rendering is done in BeginFrame/EndFrame
}

bool RaylibRenderer::Initialize() {
    // Initialize window
    InitWindow(viewportWidth, viewportHeight, "Game Engine - Raylib Renderer");
    
    // Set target FPS
    SetTargetFPS(60);
    
    // Initialize OpenGL state
    rlEnableDepthTest();
    rlEnableBackfaceCulling();
    
    return IsWindowReady();
}

void RaylibRenderer::Shutdown() {
    CleanupResources();
    CloseWindow();
}

void RaylibRenderer::BeginFrame() {
    ResetStatistics();
    BeginDrawing();
    ClearBackground(BLACK);
}

void RaylibRenderer::EndFrame() {
    EndDrawing();
}

void RaylibRenderer::Present() {
    // Raylib handles presentation automatically
}

void RaylibRenderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    viewportX = x;
    viewportY = y;
    viewportWidth = width;
    viewportHeight = height;
    SetWindowSize(width, height);
}

void RaylibRenderer::GetViewport(uint32_t& x, uint32_t& y, uint32_t& width, uint32_t& height) const {
    x = viewportX;
    y = viewportY;
    width = viewportWidth;
    height = viewportHeight;
}

void RaylibRenderer::ClearColor(const glm::vec4& color) {
    ClearBackground(ToRaylibColor(color));
}

void RaylibRenderer::ClearDepth(float depth) {
    // Raylib handles depth buffer automatically
}

void RaylibRenderer::ClearStencil(int32_t stencil) {
    // Raylib doesn't expose stencil clearing directly
}

void RaylibRenderer::Clear(bool color, bool depth, bool stencil) {
    if (color) {
        ClearBackground(BLACK);
    }
    // Depth and stencil clearing are handled by Raylib
}

TextureHandle RaylibRenderer::CreateTexture(const TextureDesc& desc) {
    Texture2D texture = LoadTextureFromMemory(desc.data, desc.width * desc.height * desc.channels);
    
    if (texture.id <= 0) {
        std::cerr << "Failed to create texture!" << std::endl;
        return 0;
    }
    
    TextureHandle handle = nextTextureHandle++;
    textures[handle] = texture;
    
    return handle;
}

ShaderHandle RaylibRenderer::CreateShader(const ShaderDesc& desc) {
    Shader shader = LoadShaderFromMemory(desc.vertexSource.c_str(), desc.fragmentSource.c_str());
    
    if (shader.id <= 0) {
        std::cerr << "Failed to create shader!" << std::endl;
        return 0;
    }
    
    ShaderHandle handle = nextShaderHandle++;
    shaders[handle] = shader;
    
    return handle;
}

MeshHandle RaylibRenderer::CreateMesh(const MeshDesc& desc) {
    Mesh mesh = {0};
    
    // Set vertices
    mesh.vertexCount = desc.vertices.size() / 3;
    mesh.vertices = (float*)MemAlloc(desc.vertices.size() * sizeof(float));
    memcpy(mesh.vertices, desc.vertices.data(), desc.vertices.size() * sizeof(float));
    
    // Set indices if available
    if (!desc.indices.empty()) {
        mesh.triangleCount = desc.indices.size() / 3;
        mesh.indices = (unsigned short*)MemAlloc(desc.indices.size() * sizeof(unsigned short));
        for (size_t i = 0; i < desc.indices.size(); i++) {
            mesh.indices[i] = (unsigned short)desc.indices[i];
        }
    }
    
    // Set normals if available
    if (!desc.normals.empty()) {
        mesh.normals = (float*)MemAlloc(desc.normals.size() * sizeof(float));
        memcpy(mesh.normals, desc.normals.data(), desc.normals.size() * sizeof(float));
    }
    
    // Set texture coordinates if available
    if (!desc.texCoords.empty()) {
        mesh.texcoords = (float*)MemAlloc(desc.texCoords.size() * sizeof(float));
        memcpy(mesh.texcoords, desc.texCoords.data(), desc.texCoords.size() * sizeof(float));
    }
    
    // Upload to GPU
    UploadMesh(&mesh, false);
    
    MeshHandle handle = nextMeshHandle++;
    meshes[handle] = mesh;
    
    return handle;
}

MaterialHandle RaylibRenderer::CreateMaterial(const MaterialDesc& desc) {
    auto material = std::make_unique<MaterialData>();
    material->shader = desc.shader;
    material->floatParams = desc.floatParams;
    material->vec3Params = desc.vec3Params;
    material->vec4Params = desc.vec4Params;
    material->textureParams = desc.textureParams;
    
    MaterialHandle handle = nextMaterialHandle++;
    materials[handle] = std::move(material);
    
    return handle;
}

void RaylibRenderer::DestroyTexture(TextureHandle handle) {
    auto it = textures.find(handle);
    if (it != textures.end()) {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

void RaylibRenderer::DestroyShader(ShaderHandle handle) {
    auto it = shaders.find(handle);
    if (it != shaders.end()) {
        UnloadShader(it->second);
        shaders.erase(it);
    }
}

void RaylibRenderer::DestroyMesh(MeshHandle handle) {
    auto it = meshes.find(handle);
    if (it != meshes.end()) {
        UnloadMesh(it->second);
        meshes.erase(it);
    }
}

void RaylibRenderer::DestroyMaterial(MaterialHandle handle) {
    auto it = materials.find(handle);
    if (it != materials.end()) {
        materials.erase(it);
    }
}

void RaylibRenderer::DrawMesh(MeshHandle mesh, MaterialHandle material, const glm::mat4& transform) {
    auto meshIt = meshes.find(mesh);
    auto materialIt = materials.find(material);
    
    if (meshIt == meshes.end() || materialIt == materials.end()) {
        return;
    }
    
    auto shaderIt = shaders.find(materialIt->second->shader);
    if (shaderIt == shaders.end()) {
        return;
    }
    
    // Set shader uniforms
    Shader shader = shaderIt->second;
    
    // Set transform matrix
    Matrix matTransform = ToRaylibMatrix(transform);
    SetShaderValueMatrix(shader, GetShaderLocation(shader, "transform"), matTransform);
    
    // Set view and projection matrices
    SetShaderValueMatrix(shader, GetShaderLocation(shader, "view"), ToRaylibMatrix(viewMatrix));
    SetShaderValueMatrix(shader, GetShaderLocation(shader, "projection"), ToRaylibMatrix(projectionMatrix));
    
    // Set material parameters
    for (const auto& param : materialIt->second->floatParams) {
        int loc = GetShaderLocation(shader, param.first.c_str());
        if (loc != -1) {
            SetShaderValue(shader, loc, &param.second, SHADER_UNIFORM_FLOAT);
        }
    }
    
    for (const auto& param : materialIt->second->vec3Params) {
        int loc = GetShaderLocation(shader, param.first.c_str());
        if (loc != -1) {
            float values[3] = {param.second.x, param.second.y, param.second.z};
            SetShaderValue(shader, loc, values, SHADER_UNIFORM_VEC3);
        }
    }
    
    for (const auto& param : materialIt->second->vec4Params) {
        int loc = GetShaderLocation(shader, param.first.c_str());
        if (loc != -1) {
            float values[4] = {param.second.x, param.second.y, param.second.z, param.second.w};
            SetShaderValue(shader, loc, values, SHADER_UNIFORM_VEC4);
        }
    }
    
    // Draw mesh
    BeginMode3D({0});
    rlEnableShader(shader.id);
    DrawMesh(meshIt->second, shader, WHITE);
    rlDisableShader();
    EndMode3D();
    
    drawCalls++;
    triangleCount += meshIt->second.triangleCount;
}

void RaylibRenderer::DrawMeshInstanced(MeshHandle mesh, MaterialHandle material, const glm::mat4* transforms, uint32_t count) {
    // Simplified instanced rendering - draw each instance separately
    for (uint32_t i = 0; i < count; i++) {
        DrawMesh(mesh, material, transforms[i]);
    }
}

void RaylibRenderer::DrawFullscreenQuad(MaterialHandle material) {
    // TODO: Implement fullscreen quad rendering
}

void RaylibRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {
    DrawLine3D(ToRaylibVector(start), ToRaylibVector(end), ToRaylibColor(color));
    drawCalls++;
}

void RaylibRenderer::DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color) {
    Vector3 size = {
        (max.x - min.x) * 0.5f,
        (max.y - min.y) * 0.5f,
        (max.z - min.z) * 0.5f
    };
    Vector3 center = {
        min.x + size.x,
        min.y + size.y,
        min.z + size.z
    };
    
    DrawCubeWires(center, size.x * 2, size.y * 2, size.z * 2, ToRaylibColor(color));
    drawCalls++;
}

void RaylibRenderer::DrawSphere(const glm::vec3& center, float radius, const glm::vec3& color) {
    DrawSphereWires(ToRaylibVector(center), radius, 16, 16, ToRaylibColor(color));
    drawCalls++;
}

void RaylibRenderer::DrawGrid(const glm::vec3& center, float size, float step, const glm::vec3& color) {
    // Draw grid on XZ plane
    int gridSize = static_cast<int>(size / step);
    rlPushMatrix();
    rlTranslatef(center.x, center.y, center.z);
    DrawGrid(gridSize, step);
    rlPopMatrix();
    drawCalls++;
}

void RaylibRenderer::SetRenderState(const RenderState& state) {
    currentRenderState = state;
    
    // Apply render state
    if (state.depthTest) {
        rlEnableDepthTest();
    } else {
        rlDisableDepthTest();
    }
    
    if (state.cullFace) {
        rlEnableBackfaceCulling();
    } else {
        rlDisableBackfaceCulling();
    }
}

const RenderState& RaylibRenderer::GetRenderState() const {
    return currentRenderState;
}

void RaylibRenderer::SetViewMatrix(const glm::mat4& view) {
    viewMatrix = view;
}

void RaylibRenderer::SetProjectionMatrix(const glm::mat4& projection) {
    projectionMatrix = projection;
}

const glm::mat4& RaylibRenderer::GetViewMatrix() const {
    return viewMatrix;
}

const glm::mat4& RaylibRenderer::GetProjectionMatrix() const {
    return projectionMatrix;
}

void RaylibRenderer::SetAmbientLight(const glm::vec3& color) {
    ambientLight = color;
}

const glm::vec3& RaylibRenderer::GetAmbientLight() const {
    return ambientLight;
}

void RaylibRenderer::SetWindowTitle(const std::string& title) {
    SetWindowTitle(title.c_str());
}

void RaylibRenderer::SetWindowSize(uint32_t width, uint32_t height) {
    SetWindowSize(width, height);
    viewportWidth = width;
    viewportHeight = height;
}

void RaylibRenderer::GetWindowSize(uint32_t& width, uint32_t& height) const {
    width = viewportWidth;
    height = viewportHeight;
}

bool RaylibRenderer::ShouldClose() const {
    return WindowShouldClose();
}

float RaylibRenderer::GetDeltaTime() const {
    return GetFrameTime();
}

float RaylibRenderer::GetTotalTime() const {
    return GetTime();
}

void RaylibRenderer::SetVSync(bool enabled) {
    SetTargetFPS(enabled ? 60 : 0);
}

bool RaylibRenderer::GetVSync() const {
    return GetTargetFPS() > 0;
}

uint32_t RaylibRenderer::GetDrawCalls() const {
    return drawCalls;
}

uint32_t RaylibRenderer::GetTriangleCount() const {
    return triangleCount;
}

void RaylibRenderer::ResetStatistics() {
    drawCalls = 0;
    triangleCount = 0;
}

// Helper functions
Color RaylibRenderer::ToRaylibColor(const glm::vec3& color) {
    return {
        static_cast<unsigned char>(color.r * 255),
        static_cast<unsigned char>(color.g * 255),
        static_cast<unsigned char>(color.b * 255),
        255
    };
}

Color RaylibRenderer::ToRaylibColor(const glm::vec4& color) {
    return {
        static_cast<unsigned char>(color.r * 255),
        static_cast<unsigned char>(color.g * 255),
        static_cast<unsigned char>(color.b * 255),
        static_cast<unsigned char>(color.a * 255)
    };
}

Vector3 RaylibRenderer::ToRaylibVector(const glm::vec3& vec) {
    return {vec.x, vec.y, vec.z};
}

Matrix RaylibRenderer::ToRaylibMatrix(const glm::mat4& mat) {
    return {
        mat[0][0], mat[0][1], mat[0][2], mat[0][3],
        mat[1][0], mat[1][1], mat[1][2], mat[1][3],
        mat[2][0], mat[2][1], mat[2][2], mat[2][3],
        mat[3][0], mat[3][1], mat[3][2], mat[3][3]
    };
}

void RaylibRenderer::CleanupResources() {
    // Destroy all textures
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
    textures.clear();
    
    // Destroy all shaders
    for (auto& pair : shaders) {
        UnloadShader(pair.second);
    }
    shaders.clear();
    
    // Destroy all meshes
    for (auto& pair : meshes) {
        UnloadMesh(pair.second);
    }
    meshes.clear();
    
    // Clear materials
    materials.clear();
}

} // namespace GameEngine