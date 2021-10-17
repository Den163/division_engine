#include <entt/entity/registry.hpp>
#include "gl_render_system.h"

#include "../../../utils/debug_utils.h"
#include "../../../utils/math.h"
#include "../../components/gui_mesh.h"
#include "../../components/gl_mesh.h"
#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"

static inline void applyVertexBuffer(const GlMesh& glMesh, const GuiMesh& mesh);
static inline void applyTransformMatrix(const glm::mat4& modelMatrix, const glm::mat4& projectionMatrix);

void GlRenderGuiSystem::init(GlShaderState& shaderState, CameraState& cameraState, const WindowState& windowState)
{
    glEnable(GL_DEBUG_OUTPUT);
    glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
    glDebugMessageCallback(DebugUtils::glRendererMessageCallback, nullptr);

    glCreateVertexArrays(GlShaderState::VERTEX_ARRAYS_COUNT, shaderState.vertexArrayHandles);


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
    auto& vaoHandle = shaderState.vertexArrayHandles[GlShaderState::TRIANGLES_ARRAY_INDEX];
    auto projectionMatrix = glm::ortho(0.f, (float) windowState.width, 0.f, (float) windowState.height);

    glViewport(0, 0, windowState.width, windowState.height);
    glClearBufferfv(GL_COLOR, 0, reinterpret_cast<const GLfloat*>(&rendererState.backgroundColor));

    auto meshComponentsView = guiRegistry.view<GuiMesh, GlMesh, const Position, const Rotation, const Scale>();
    for (auto&& [e, mesh, glMesh, pos, rot, scale]: meshComponentsView.each())
    {
        auto modelMatrix = Math::transformMatrix(pos.value, rot.value, scale.value);

        glBindVertexArray(vaoHandle);

        applyVertexBuffer(glMesh, mesh);
        applyTransformMatrix(modelMatrix, projectionMatrix);

        glDrawArrays(static_cast<GLenum>(mesh.renderShape), 0, static_cast<GLsizei>(mesh.vertices.size()));
    }
}

static inline void applyVertexBuffer(const GlMesh& glMesh, const GuiMesh& mesh)
{
    const auto& vertexVboHandle = glMesh.vboHandle;
    const auto& vertices = mesh.vertices;
    const auto* verticesPtr = vertices.data();
    const auto verticesCount = vertices.size();
    const auto verticesBytes = static_cast<GLsizeiptr>(verticesCount * sizeof(GuiVertex));

    glBindBuffer(GL_ARRAY_BUFFER, vertexVboHandle);

    GLint bufferSize;
    glGetNamedBufferParameteriv(vertexVboHandle, GL_BUFFER_SIZE, &bufferSize);

    if (bufferSize == verticesBytes)
    {
        glNamedBufferSubData(vertexVboHandle, 0, verticesBytes, verticesPtr);
    }
    else
    {
        glNamedBufferStorage(vertexVboHandle, verticesBytes, verticesPtr, GL_DYNAMIC_STORAGE_BIT);
    }

    glVertexAttribPointer(
        GlMesh::VERTEX_ATTRIB_INDEX,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        (void*) offsetof(GuiVertex, position));

    glVertexAttribPointer(
        GlMesh::COLOR_ATTRIB_INDEX,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        (void*) offsetof(GuiVertex, color));


    glEnableVertexAttribArray(GlMesh::VERTEX_ATTRIB_INDEX);
    glEnableVertexAttribArray(GlMesh::COLOR_ATTRIB_INDEX);
}

static inline void applyTransformMatrix(const glm::mat4& modelMatrix, const glm::mat4& projectionMatrix)
{
    auto transformMatrix = projectionMatrix * modelMatrix;

    glUniformMatrix4fv(GlMesh::MVP_MATRIX_UNIFORM_INDEX, 1, GL_FALSE, &transformMatrix[0][0]);
}
