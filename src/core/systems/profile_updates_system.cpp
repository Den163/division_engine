#include "profile_updates_system.h"

#include <iostream>

#include "../components/renderer_state.h"
#include "../components/window_config.h"

void ProfileUpdatesSystem::init(singleton_registry& engineData)
{
}

void ProfileUpdatesSystem::update(singleton_registry& engineData)
{
    auto [ cfg, render ] = engineData.get<WindowConfig,  const RendererState>();
    // TODO: write some profile code
}

void ProfileUpdatesSystem::cleanup(singleton_registry& engineData)
{
}