#include "gui_composer.h"

#include "../../events/gui_mesh_created.h"
#include "../../events/gui_mesh_destroyed.h"
#include "../../components/hierarchy.h"
#include "../../components/gui_anchor.h"
#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"

static void checkGuiRoot(EngineState& state, entt::entity entity);

GuiComposer::GuiComposer(EngineState& engineState) : engineState(engineState) {}

entt::entity GuiComposer::makeGuiElement(const Transform& transform)
{
    auto& registry = engineState.guiRegistry;
    const auto& e = registry.create();
    auto& pos = registry.emplace<Position>(e);
    pos.value = transform.position;

    auto& rot = registry.emplace<Rotation>(e);
    rot.value = transform.position;

    auto& scale = registry.emplace<Scale>(e);
    scale.value = transform.scale;

    auto& anchor = registry.emplace<GuiAnchor>(e);
    anchor.child = {0.5f, 0.5f};
    anchor.parent = {0.5f, 0.5f};

    registry.emplace<GuiMeshCreated>(e);
    registry.emplace<GuiMeshDestroyed>(e);

    checkGuiRoot(engineState, e);
    return e;
}

entt::entity GuiComposer::makeGuiRect(const GuiRect& guiRect, const GuiRectColor& color, const Transform transform)
{
    const auto e = makeGuiElement(transform);
    return e;
}

void checkGuiRoot(EngineState& state, entt::entity entity)
{
    if (state.guiRootElement == entt::null)
    {
        state.guiRootElement = entity;
    }
}