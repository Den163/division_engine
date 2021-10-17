#include "lifecycle.h"

#include <iostream>
#include <random>
#include <stdexcept>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../core/components/gui_mesh.h"
#include "../utils/gui_primitive_factory.h"
#include "../utils/debug_utils.h"
#include "../utils/math.h"

static inline void checkMeshCreateByKeyPress(EngineState& state);
static inline void checkMeshDeleteByKeyPress(EngineState& state);
static inline void checkPrintCameraInfoByKeyPress(EngineState& state);
static inline glm::vec3 randomVert(float minX, float maxX, float minY, float maxY);

void Lifecycle::init(EngineState& state)
{
    auto& cameraState = state.cameraState;

    auto [ae, axisMesh] = GuiPrimitiveFactory::createTriangle(
        state.guiRegistry,
        Transform { {}, {}, glm::vec3{1.f} },
        { glm::vec3 { 100,100,-0.5f }, {100,200,-0.5f}, {200,200,-0.5f} },
        glm::vec4 {1,0,0,1}
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
    auto [e, mesh] = GuiPrimitiveFactory::createMeshEntity(state.guiRegistry);

    mesh.renderShape = RenderMode::Triangles;
    mesh.vertices = {
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight), },
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight) },
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight) },
    };
}

void checkMeshDeleteByKeyPress(EngineState& state)
{
    if (!state.inputState.keyboardState.keyHasFlag('D', InputState::KEY_STATE_PRESSED)) return;

    for (auto&& [e, mesh] : state.guiRegistry.view<GuiMesh>().each())
    {
        state.guiRegistry.remove<GuiMesh>(e);
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