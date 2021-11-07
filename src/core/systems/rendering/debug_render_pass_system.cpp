#include "debug_render_pass_system.h"

void DebugRenderPassSystem::init(EngineState& engineState)
{
    auto& renderPassUpdate = engineState.renderer.renderPass;
    preRender(engineState);
    postRender(engineState);
}

void DebugRenderPassSystem::preRender(EngineState& engineState)
{
    engineState.renderer.renderPass.lastUpdateTime = std::chrono::steady_clock::now();
}

void DebugRenderPassSystem::postRender(EngineState& engineState)
{
    auto& renderPassUpdate = engineState.renderer.renderPass;
    renderPassUpdate.deltaTime = std::chrono::steady_clock::now() - renderPassUpdate.lastUpdateTime;
}
