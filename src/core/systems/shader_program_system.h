#pragma once

#include "../../utils/singleton_registry.h"

namespace ShaderProgramSystem
{
void init(singleton_registry& singletonRegistry);
void cleanup(singleton_registry& singletonRegistry);
}