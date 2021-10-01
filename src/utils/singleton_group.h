#pragma once

#include <entt/entt.hpp>

template<typename... TGroupArgs>
class singleton_group
{
public:
    singleton_group(entt::entity e, entt::group<TGroupArgs...> g) : singleton(e), group(g) {}

    template<typename... TGetGroup>
    decltype(auto) get() { return group.template get<TGetGroup...>(singleton); }

private:
    entt::entity singleton;
    entt::group<TGroupArgs...>  group;
};