#include "RaylibRenderer.hpp"
#include "raylib.h"

bool RaylibRenderer::Initialize(EngineCore& engine) {
    return Initialize();
}

bool RaylibRenderer::Initialize() {
    InitWindow(800, 600, "Game Engine");
    return true;
}

void RaylibRenderer::Shutdown() {
    CloseWindow();
}

void RaylibRenderer::Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
}

void RaylibRenderer::BeginFrame() {}
void RaylibRenderer::EndFrame() {}
TextureHandle RaylibRenderer::CreateTexture(const TextureDesc& desc) { return 0; }
ShaderHandle RaylibRenderer::CreateShader(const ShaderDesc& desc) { return 0; }
MeshHandle RaylibRenderer::CreateMesh(const MeshDesc& desc) { return 0; }
void RaylibRenderer::DrawMesh(MeshHandle mesh, const glm::mat4& transform) {}
void RaylibRenderer::DrawFullscreenQuad(ShaderHandle shader) {}
void RaylibRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {}
