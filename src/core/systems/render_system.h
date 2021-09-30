#pragma once

#include "../../utils/singleton_registry.h"
#include "../components/rendered_frame_state.h"

namespace RenderSystem
{
void init(singleton_registry& engineData);
void update(singleton_registry& engineData);
void cleanup(singleton_registry& engineData);
}