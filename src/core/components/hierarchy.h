#pragma once

#include <entt/entt.hpp>

struct Hierarchy
{
    size_t childrenCount = 0;
    entt::entity nextSibling = entt::null;
    entt::entity prevSibling = entt::null;
    entt::entity firstChild = entt::null;
    entt::entity parent = entt::null;
};