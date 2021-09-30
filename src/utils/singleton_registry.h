#pragma once

#include <entt/entt.hpp>

class singleton_registry
{
public:
    singleton_registry() :
        registry(entt::registry {}),
        singleton(registry.create()) {}

    template<typename... TComponent>
    decltype(auto) get() { return registry.get<TComponent...>(singleton);  }

    template<typename T>
    decltype(auto) emplace() { return registry.emplace<T>(singleton); }

    template<typename... TComponent>
    void erase() { registry.erase<TComponent...>(singleton); }

private:
    entt::registry registry;
    entt::entity singleton;
};


