#include "lifecycle.h"

#include <cstdlib>
#include <ctime>
#include <random>

void Lifecycle::preInit(EngineState& state)
{
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
    auto& vertexBuffer = shaderState.vertexBuffer;

    std::random_device randomDevice {};
    std::mt19937 generator {randomDevice()};
    auto tDist = std::uniform_real_distribution { 0.0, 1.0 };
    auto indexDist = std::uniform_int_distribution<size_t> { 0, vertexBuffer.size() - 1 };
    auto t = (float) tDist(generator);
    auto randomValue = (1-t) - t;
    auto randomIndex = indexDist(generator);

    vertexBuffer[randomIndex] = randomValue;
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
