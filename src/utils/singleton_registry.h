#pragma once

#include <entt/entt.hpp>

class singleton_registry
{
public:
    singleton_registry() :
        registry(entt::registry {}),
        singleton(registry.create()) {}

    template<typename... TComponent>
    decltype(auto) get() { return registry.template get<TComponent...>(singleton);  }

    template<typename TComponent>
    decltype(auto) emplace() { return registry.template emplace<TComponent>(singleton); }

    template<typename... TComponent>
    void erase() { registry.template erase<TComponent...>(singleton); }

private:
    entt::registry registry;
    entt::entity singleton;
};


