#include "lifecycle.h"

#include <iostream>
#include <random>
#include <stdexcept>

#include "../core/components/mesh.h"
#include "../utils/primitive_factory.h"

static void checkMeshCreateByKeyPress(EngineState& state);
static void checkMeshDeleteByKeyPress(EngineState& state);
static glm::vec3 randomVert(float minX, float maxX, float minY, float maxY);

void Lifecycle::init(EngineState& state)
{
    auto [e, mesh] = PrimitiveFactory::createMeshEntity(state.ecsRegistry);

    mesh.vertices = {
        Mesh::Vertex{{-300, -300, 0.0}},
        Mesh::Vertex{{ 250, -300, 0.0}},
        Mesh::Vertex{{-300,  250, 0.0}},
        Mesh::Vertex{{ 300, -250, 0.0}},
        Mesh::Vertex{{ 300,  300, 0.0}},
        Mesh::Vertex{{-250,  300, 0.0}},
    };
}

void Lifecycle::preRenderUpdate(EngineState& state)
{
}

void Lifecycle::postRenderUpdate(EngineState& state)
{
    glm::vec3 move { 100, 0, 0 };

    if (state.inputState.keyboardState.keyHasFlag('J', InputState::KEY_STATE_PRESSED))
    {
        state.cameraState.position -= move;
        std::cout << "Moving left" << std::endl;
    }

    if (state.inputState.keyboardState.keyHasFlag('K', InputState::KEY_STATE_PRESSED))
    {
        state.cameraState.position += move;
        std::cout << "Moving right" << std::endl;
    }

    checkMeshCreateByKeyPress(state);
    checkMeshDeleteByKeyPress(state);
}

void checkMeshCreateByKeyPress(EngineState& state)
{
    if (!state.inputState.keyboardState.keyHasFlag('C', InputState::KEY_STATE_PRESSED)) return;

    auto halfWidth = state.windowState.width * 0.5f;
    auto halfHeight = state.windowState.height * 0.5f;
    auto [e, mesh] = PrimitiveFactory::createMeshEntity(state.ecsRegistry);
    mesh.vertices = {
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight) },
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight) },
        {randomVert(-halfWidth, halfWidth, -halfHeight, halfHeight) },
    };
}

void checkMeshDeleteByKeyPress(EngineState& state)
{
    if (!state.inputState.keyboardState.keyHasFlag('D', InputState::KEY_STATE_PRESSED)) return;

    for (auto&& [e, mesh] : state.ecsRegistry.view<Mesh>().each())
    {
        state.ecsRegistry.remove<Mesh>(e);
        return;
    }
}

glm::vec3 randomVert(float minX, float maxX, float minY, float maxY)
{
    std::uniform_real_distribution<float> dists[2] = {
        std::uniform_real_distribution { minX, maxX },
        std::uniform_real_distribution { minY, maxY },
    };

    glm::vec3 v;

    for (size_t i = 0; i < 2; i++)
    {
        auto randomDevice = std::random_device {};
        auto randomFunctor = std::mt19937 { randomDevice() };
        v[i] = dists[i](randomFunctor);
    }

    v[2] = 1.0f;

    return v;
}

void Lifecycle::cleanup(EngineState& state)
{

}

