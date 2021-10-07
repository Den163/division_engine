#include "lifecycle.h"

#include <iostream>
#include <random>
#include <stdexcept>

#include "../core/components/mesh.h"
#include "../utils/primitive_factory.h"

static void checkMeshCreateByKeyPress(EngineState& state);
static void checkMeshDeleteByKeyPress(EngineState& state);
static glm::vec3 randomVert(EngineState& state);

void Lifecycle::init(EngineState& state)
{
    auto [e, mesh] = PrimitiveFactory::createMeshEntity(state.ecsRegistry);

    mesh.vertices = {
        Mesh::Vertex {{-0.90, -0.90, 0.0,}},
        Mesh::Vertex {{ 0.85, -0.90, 0.0,}},
        Mesh::Vertex {{-0.90,  0.85, 0.0,}},
        Mesh::Vertex {{ 0.90, -0.85, 0.0,}},
        Mesh::Vertex {{ 0.90,  0.90, 0.0,}},
        Mesh::Vertex {{-0.85,  0.90, 0.0,}},
    };
}

void Lifecycle::preRenderUpdate(EngineState& state)
{
}

void Lifecycle::postRenderUpdate(EngineState& state)
{
    if (state.inputState.keyboardState.keyHasFlag('J', InputState::KEY_STATE_PRESSED))
    {
        state.cameraState.position -= glm::vec3 { -0.1, 0, 0 };
    }

    if (state.inputState.keyboardState.keyHasFlag('K', InputState::KEY_STATE_PRESSED))
    {
        state.cameraState.position += glm::vec3 { -0.2, 0, 0 };
    }

    checkMeshCreateByKeyPress(state);
    checkMeshDeleteByKeyPress(state);
}

void checkMeshCreateByKeyPress(EngineState& state)
{
    if (!state.inputState.keyboardState.keyHasFlag('C', InputState::KEY_STATE_PRESSED)) return;

    auto [e, mesh] = PrimitiveFactory::createMeshEntity(state.ecsRegistry);
    mesh.vertices = {
        {randomVert(state) },
        {randomVert(state) },
        {randomVert(state) },
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

glm::vec3 randomVert(EngineState& state)
{
    std::uniform_real_distribution dist { -1.f, 1.f };
    glm::vec3 v;

    for (size_t i = 0; i < 3; i++)
    {
        auto randomDevice = std::random_device {};
        auto randomFunctor = std::mt19937 { randomDevice() };
        v[i] = dist(randomFunctor);
    }

    return v;
}

void Lifecycle::cleanup(EngineState& state)
{

}

