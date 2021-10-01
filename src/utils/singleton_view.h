#pragma once

#include <entt/entt.hpp>

template<typename... TViewTypeArgs>
class singleton_view
{
public:
    singleton_view(entt::entity e, entt::view<TViewTypeArgs...> v) : singleton(e), view(v) {}

    template<typename... TGetComponent>
    decltype(auto) get() { return view.template get<TGetComponent...>(singleton); }
private:
    entt::entity singleton;
    entt::view<TViewTypeArgs...> view;
};