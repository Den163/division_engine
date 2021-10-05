#include "lifecycle.h"

#include "../core/components/mesh.h"
#include <iostream>
#include <random>
#include <stdexcept>

static void checkMeshCreateByKeyPress(EngineState& state);
static void checkMeshDeleteByKeyPress(EngineState& state);

void Lifecycle::init(EngineState& state)
{
    auto e = state.ecsRegistry.create();
    auto& mesh = state.ecsRegistry.emplace<Mesh>(e);

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

void checkMeshCreateByKeyPress(EngineState& state)
{
    if (!state.inputState.keyboardState.keyHasFlag('C', InputState::KEY_STATE_PRESSED)) return;

    auto e = state.ecsRegistry.create();
    auto& mesh = state.ecsRegistry.emplace<Mesh>(e);
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

void Lifecycle::postRenderUpdate(EngineState& state)
{
    checkMeshCreateByKeyPress(state);
    checkMeshDeleteByKeyPress(state);
}

void Lifecycle::cleanup(EngineState& state)
{

}

