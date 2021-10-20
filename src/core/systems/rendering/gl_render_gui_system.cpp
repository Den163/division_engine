#include "gl_render_gui_system.h"

#include <entt/entity/registry.hpp>

#include "../../../utils/debug_utils.h"
#include "../../../utils/math.h"
#include "../../components/gui_mesh.h"
#include "../../components/gl_mesh.h"
#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"

void GlRenderGuiSystem::init(EngineState& engineState)
{
    auto& cameraState = engineState.cameraState;

    glEnable(GL_DEBUG_OUTPUT);
    glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
    glDebugMessageCallback(DebugUtils::glRendererMessageCallback, nullptr);

    cameraState.position = glm::vec3 {0};
    cameraState.rotation = glm::identity<glm::quat>();
    cameraState.nearPlane = 0.1f;
    cameraState.farPlane = 100;
}

void GlRenderGuiSystem::update(EngineState& engineState)
{
    auto& guiRegistry = engineState.guiRegistry;

    const auto& shaderState = engineState.shaderState;
    const auto& windowState = engineState.windowState;
    const auto& vaoHandle = shaderState.vaoHandle;
    const auto& projectionMatrix = glm::ortho(0.f, (float) windowState.width, 0.f, (float) windowState.height);
    const auto& renderComponentsView = guiRegistry.view<GuiMesh, GlMesh, const Position, const Rotation, const Scale>();

    auto objectsToRender = 0;
    for (auto&& [e, mesh, glMesh, pos, rot, scale]: renderComponentsView.each())
    {
        const auto& vertexVboHandle = glMesh.vertexVboHandle;
        const auto& vertices = mesh.vertices;
        const auto verticesSize = vertices.size();
        const auto& modelMatrix = Math::transformMatrix(pos.value, rot.value, scale.value);
        const auto& mvpMatrix = projectionMatrix * modelMatrix;
        mesh.modelViewProjection = mvpMatrix;

        glUseProgram(shaderState.programHandle);
        glBindVertexArray(vaoHandle);
        glBindBuffer(GL_ARRAY_BUFFER, glMesh.modelViewProjectionVboHandle);
        glNamedBufferSubData(
            glMesh.modelViewProjectionVboHandle, 0, sizeof(glm::mat4), &mesh.modelViewProjection[0][0]);

        for (size_t i = 0; i < 4; i++)
        {
            constexpr auto rowLength = glm::mat4::row_type::length();
            auto attribId = 2 + i;
            glVertexAttribPointer(
                attribId, rowLength, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (i * rowLength * sizeof(float)));
            glEnableVertexAttribArray(attribId);
            glVertexAttribDivisor(attribId, verticesSize);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertexVboHandle);

        const auto* newBufferPtr = vertices.data();
        const auto newBufferSize = vertices.size() * sizeof(GuiVertex);

        GLint currentBufferSize;
        glGetNamedBufferParameteriv(vertexVboHandle, GL_BUFFER_SIZE, &currentBufferSize);

        if (currentBufferSize == newBufferSize)
        {
            glNamedBufferSubData(vertexVboHandle, 0, newBufferSize, newBufferPtr);
        }
        else
        {
            glNamedBufferStorage(vertexVboHandle, newBufferSize, newBufferPtr, GL_DYNAMIC_STORAGE_BIT);
        }

        glEnableVertexAttribArray(GlMesh::VERTEX_ATTRIB_INDEX);
        glVertexAttribPointer(
            GlMesh::VERTEX_ATTRIB_INDEX,
            glm::vec3::length(),
            GL_FLOAT,
            GL_FALSE,
            sizeof(GuiVertex),
            (void*) offsetof(GuiVertex, position));

        glEnableVertexAttribArray(GlMesh::COLOR_ATTRIB_INDEX);
        glVertexAttribPointer(
            GlMesh::COLOR_ATTRIB_INDEX,
            glm::vec4::length(),
            GL_FLOAT,
            GL_FALSE,
            sizeof(GuiVertex),
            (void*) offsetof(GuiVertex, color));


        glDrawArrays((GLenum) mesh.renderMode, 0, (GLsizei) vertices.size());

        objectsToRender++;
    }
}