#include "gui_primitive_factory.h"

#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"
#include "../../states/shader_pipeline_state.h"
#include "../../events/gui_mesh_created.h"
#include "../../events/gui_mesh_destroyed.h"
#include "../../components/gl_texture.h"
#include "../../utils/engine_state_helper.h"

GuiMesh& GuiPrimitiveFactory::makeEntityMesh(
    EngineState& engineState, const entt::entity& entity, const Transform& transform)
{
    auto& registry = engineState.guiRegistry;
    auto& mesh = registry.emplace<GuiMesh>(entity);
    mesh.shaderPipelineHandle = EngineStateHelper::standardShaderPipeline(engineState).handle;
    mesh.vertexShaderHandle = EngineStateHelper::standardVertexShaderProgram(engineState).programHandle;
    mesh.fragmentShaderHandle = EngineStateHelper::standardColorFragmentShaderProgram(engineState).programHandle;

    auto& newPos = registry.emplace<Position>(entity);
    auto& newRot = registry.emplace<Rotation>(entity);
    auto& newScale = registry.emplace<Scale>(entity);

    newPos.value = transform.position;
    newRot.value = transform.rotation;
    newScale.value = transform.scale;

    registry.emplace<GuiMeshCreated>(entity);

    return mesh;
}


void GuiPrimitiveFactory::deleteMeshEntity(EngineState& engineState, const entt::entity& entity)
{
    engineState.guiRegistry.emplace<GuiMeshDestroyed>(entity);
}

GuiMesh& GuiPrimitiveFactory::makeEntityTriangle(
    EngineState& engineState, const entt::entity& entity, const Transform& transform, const GuiTriangle& triangle)
{
    auto& mesh = makeEntityMesh(engineState, entity, transform);
    mesh.renderMode = RenderMode::Triangles;
    mesh.vertices = std::vector<GuiVertex> { triangle.vertices.begin(), triangle.vertices.end() };

    return mesh;
}

GuiMesh& GuiPrimitiveFactory::makeEntityQuad(
    EngineState& engineState, const entt::entity& entity, const Transform& transform, const GuiQuad& quad)
{
    auto& mesh = makeEntityMesh(engineState, entity, transform);
    mesh.renderMode = RenderMode::TrianglesFan;
    mesh.vertices = std::vector<GuiVertex> { quad.vertices.begin(), quad.vertices.end() };

    return mesh;
}

void GuiPrimitiveFactory::addTexture(EngineState& engineState, const entt::entity& entity, uint32_t handle)
{
    auto& tex = engineState.guiRegistry.emplace<GlTexture>(entity);
    tex.handle = handle;
}

