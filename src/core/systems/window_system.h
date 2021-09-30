#pragma once

#include "../../utils/singleton_registry.h"
#include "../components/window_state.h"

namespace WindowSystem
{
void init(singleton_registry& engineData);
void update(singleton_registry& engineData);
void cleanup(singleton_registry& engineData);
}