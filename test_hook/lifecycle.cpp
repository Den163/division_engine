#pragma once

#include "lifecycle.h"

#include <iostream>
#include <random>
#include <stdexcept>

#include "../src/core/components/gui_mesh.h"
#include "../src/core/primitives/gui/gui_primitive_factory.h"
#include "../src/utils/debug_utils.h"
#include "../src/utils/color.h"

static inline void checkMeshCreateByKeyPress(EngineState& state);
static inline void checkMeshDeleteByKeyPress(EngineState& state);
static inline void checkPrintCameraInfoByKeyPress(EngineState& state);
static inline glm::vec3 randomVert(float minX, float maxX, float minY, float maxY);

void Lifecycle::init(EngineState& state)
{
    auto& cameraState = state.cameraState;

    auto& tm = GuiPrimitiveFactory::makeEntityTriangle(
        state.guiRegistry,
        state.guiRegistry.create(),
        Transform::makeDefault(),
        GuiTriangle::create(
            {glm::vec3 {100, 100, 0.f}, {100, 200, 0.f}, {200, 200, 0.f}},
            Color::red
        ));

    auto& qm = GuiPrimitiveFactory::makeEntityQuad(
        state.guiRegistry,
        state.guiRegistry.create(),
        Transform::makeDefault().withPosition({400, 400, 0}),
        GuiQuad::create(300, 400, Color::yellow)
    );
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
    if (!state.inputState.keyboardState.keyHasFlag('C', InputState::KEY_STATE_PRESSED)) return;

    auto halfWidth = state.windowState.width * 0.5f;
    auto halfHeight = state.windowState.height * 0.5f;
    auto& mesh = GuiPrimitiveFactory::makeEntityMesh(
        state.guiRegistry,
        state.guiRegistry.create(),
        Transform::makeDefault());

    mesh.renderShape = RenderMode::Triangles;
    mesh.vertices = {
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight), Color::blue },
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight), Color::blue },
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight), Color::blue },
    };
}

void checkMeshDeleteByKeyPress(EngineState& state)
{
    if (!state.inputState.keyboardState.keyHasFlag('D', InputState::KEY_STATE_PRESSED)) return;

    for (auto&& [e, mesh] : state.guiRegistry.view<GuiMesh>().each())
    {
        GuiPrimitiveFactory::deleteMeshEntity(state.guiRegistry, e);
        return;
    }
}

glm::vec3 randomVert(float minX, float maxX, float minY, float maxY)
{
    std::uniform_real_distribution<float> dists[2] = {
        std::uniform_real_distribution { minX, maxX },
        std::uniform_real_distribution { minY, maxY },
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
    const auto& cameraState = state.cameraState;

    if (state.inputState.keyPressed('P'))
    {
       std::cout << "Camera position        : " << cameraState.position << std::endl
                 << "Camera rotation (euler): " << glm::degrees(glm::eulerAngles(cameraState.rotation)) << std::endl
                 << "Mouse screen position  : " << state.inputState.postRenderMousePosition << std::endl;
    }
}

void Lifecycle::cleanup(EngineState& state)
{

}