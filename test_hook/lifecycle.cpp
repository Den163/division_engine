#pragma once

#include "lifecycle.h"

#include <iostream>
#include <random>
#include <stdexcept>

#include "../src/core/components/gui_mesh.h"
#include "../src/core/primitives/gui/gui_primitive_factory.h"
#include "../src/utils/debug_utils.h"
#include "../src/utils/color.h"
#include "../src/utils/engine_state_helper.h"

static inline void checkMeshCreateByKeyPress(EngineState& state);
static inline void checkMeshDeleteByKeyPress(EngineState& state);
static inline void checkPrintCameraInfoByKeyPress(EngineState& state);
static inline glm::vec3 randomPos(const glm::vec2& min, const glm::vec2& max);

void Lifecycle::init(EngineState& state)
{
    auto& cameraState = state.camera;
    auto& registry = state.guiRegistry;

    auto& tm = GuiPrimitiveFactory::makeEntityTriangle(
        state.guiRegistry,
        state.guiRegistry.create(),
        Transform::makeDefault(),
        GuiTriangle::create(
            {glm::vec3 {100, 100, 0.f}, {100, 200, 0.f}, {200, 200, 0.f}},
            Color::red
        ));

    const auto& qe = state.guiRegistry.create();
    auto& qm = GuiPrimitiveFactory::makeEntityQuad(
        registry,
        qe,
        Transform::makeDefault().withPosition({400, 400, 0}),
        GuiQuad::create(300, 400, Color::yellow)
    );
    qm.fragmentShaderIndex = EngineInvariants::STANDARD_TEXTURE_FRAGMENT_SHADER_INDEX;
    GuiPrimitiveFactory::addTexture(registry, qe, EngineStateHelper::texture2d(state, 0).handle);
}

void Lifecycle::preRenderUpdate(EngineState& state)
{
}

void Lifecycle::postRenderUpdate(EngineState& state)
{
    checkPrintCameraInfoByKeyPress(state);
    checkMeshCreateByKeyPress(state);
    checkMeshDeleteByKeyPress(state);
}

void checkMeshCreateByKeyPress(EngineState& state)
{
    if (!state.input.keyboardState.keyHasFlag('C', InputState::KEY_STATE_PRESSED)) return;

    const auto& windowState = state.window;
    auto& mesh = GuiPrimitiveFactory::makeEntityMesh(
        state.guiRegistry,
        state.guiRegistry.create(),
        Transform::makeDefault());

    const auto& minPos = glm::vec2 { 0 };
    const auto& maxPos = glm::vec2 { windowState.width, windowState.height };

    mesh.renderMode = RenderMode::Triangles;

    const auto& p0 = randomPos(minPos, maxPos);
    const auto& p1 = randomPos(minPos, maxPos);
    const auto& p2 = randomPos(minPos, maxPos);

    mesh.vertices = {
        {Color::blue, p0, { 0,0 } },
        {Color::blue, p1, { 0.5f, 1 } },
        {Color::blue, p2, { 1, 1 } },
    };
}

void checkMeshDeleteByKeyPress(EngineState& state)
{
    if (!state.input.keyboardState.keyHasFlag('D', InputState::KEY_STATE_PRESSED)) return;

    for (auto&& [e, mesh] : state.guiRegistry.view<GuiMesh>().each())
    {
        GuiPrimitiveFactory::deleteMeshEntity(state.guiRegistry, e);
        return;
    }
}

glm::vec3 randomPos(const glm::vec2& min, const glm::vec2& max)
{
    std::uniform_real_distribution<float> dists[2] = {
        std::uniform_real_distribution { min.x, max.x },
        std::uniform_real_distribution { min.y, max.y },
    };

    glm::vec3 v { 0.0f };

    for (size_t i = 0; i < 2; i++)
    {
        auto randomDevice = std::random_device {};
        auto randomFunctor = std::mt19937 { randomDevice() };
        v[i] = dists[i](randomFunctor);
    }

    return v;
}

void checkPrintCameraInfoByKeyPress(EngineState& state)
{
    const auto& cameraState = state.camera;

    if (state.input.keyPressed('P'))
    {
       std::cout << "Camera position        : " << cameraState.position << std::endl
                 << "Camera rotation (euler): " << glm::degrees(glm::eulerAngles(cameraState.rotation)) << std::endl
                 << "Mouse screen position  : " << state.input.postRenderMousePosition << std::endl;
    }
}

void Lifecycle::cleanup(EngineState& state)
{

}