#include "lifecycle.h"

#include <cstdlib>
#include <ctime>

void Lifecycle::preInit(EngineState& state)
{
    std::srand(std::time(nullptr));

    state.shaderState.vertexBuffer = {
        -0.90, -0.90, 0.0,
         0.85, -0.90, 0.0,
        -0.90,  0.85, 0.0,
         0.90, -0.85, 0.0,
         0.90,  0.90, 0.0,
        -0.85,  0.90, 0.0,
    };
}

void Lifecycle::postInit(EngineState& state)
{
}

void Lifecycle::beginLoopUpdate(EngineState& state)
{

}

void Lifecycle::preRenderUpdate(EngineState& state)
{
    auto& shaderState = state.shaderState;
    auto vertexBuffer = shaderState.vertexBuffer;
    float t = std::rand() / (float) RAND_MAX;
    int idx = (int) ((1 - t) * (vertexBuffer.size() - 1));

    vertexBuffer[idx] = -t + (1-t);
}

void Lifecycle::postRenderUpdate(EngineState& state)
{

}

void Lifecycle::endLoopUpdate(EngineState& state)
{

}

void Lifecycle::preCleanup(EngineState& state)
{

}

void Lifecycle::postCleanup(EngineState& state)
{

}
