#pragma once

#include "../../utils/singleton_registry.h"

namespace RenderTickSystem
{
void init(singleton_registry& engineData);
void update(singleton_registry& engineData);
void cleanup(singleton_registry& engineData);
}