#include <entt/entity/registry.hpp>
#include "gl_render_gui_system.h"

#include "../../../utils/debug_utils.h"
#include "../../../utils/math.h"
#include "../../components/gui_mesh.h"
#include "../../components/gl_mesh.h"
#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"

#include "../../../utils/shader_query.h"

void GlRenderGuiSystem::init(GlShaderState& shaderState, CameraState& cameraState, const WindowState& windowState)
{
    glEnable(GL_DEBUG_OUTPUT);
    glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
    glDebugMessageCallback(DebugUtils::glRendererMessageCallback, nullptr);

    cameraState.position = glm::vec3 {0};
    cameraState.rotation = glm::identity<glm::quat>();
    cameraState.nearPlane = 0.1f;
    cameraState.farPlane = 100;
}

void GlRenderGuiSystem::update(
    entt::registry& guiRegistry,
    const GlShaderState& shaderState,
    const RendererState& rendererState,
    const CameraState& cameraState,
    const WindowState& windowState)
{
    auto& vaoHandle = shaderState.vaoHandle;
    auto projectionMatrix = glm::ortho(0.f, (float) windowState.width, 0.f, (float) windowState.height);

    glViewport(0, 0, windowState.width, windowState.height);
    glClearBufferfv(GL_COLOR, 0, reinterpret_cast<const GLfloat*>(&rendererState.backgroundColor));

    auto meshComponentsView = guiRegistry.view<GuiMesh, GlMesh, const Position, const Rotation, const Scale>();
    for (auto&& [e, mesh, glMesh, pos, rot, scale]: meshComponentsView.each())
    {
        const auto& vertexVboHandle = glMesh.vertexVboHandle;
        const auto& vertices = mesh.vertices;
        const auto& modelMatrix = Math::transformMatrix(pos.value, rot.value, scale.value);
        const auto& mvpMatrix = projectionMatrix * modelMatrix;

        auto shaderQuery = ShaderQuery
        {
            shaderState.programHandle,
            vaoHandle
        };

        shaderQuery.setUniform(GlMesh::MVP_MATRIX_UNIFORM_INDEX, mvpMatrix);
        shaderQuery.modifyBuffer(glMesh.vertexVboHandle, vertices)
            .bindVecAttributeToField(GlMesh::VERTEX_ATTRIB_INDEX, &GuiVertex::position)
            .bindVecAttributeToField(GlMesh::COLOR_ATTRIB_INDEX, &GuiVertex::color)
            .draw(mesh.renderShape);
    }
}