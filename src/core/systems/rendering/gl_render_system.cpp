#include <entt/entity/registry.hpp>
#include "gl_render_system.h"

#include "../../../utils/debug_utils.h"
#include "../../../utils/math.h"
#include "../../components/mesh.h"
#include "../../components/gl_mesh.h"
#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"

static inline void applyVertexBuffer(const GlMesh& glMesh, const Mesh& mesh);
static inline void applyTransformMatrix(
    const glm::mat4& modelMatrix,
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix);

void GlRenderSystem::init(GlShaderState& shaderState, CameraState& cameraState, const WindowState& windowState)
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugUtils::glRendererMessageCallback, nullptr);

    glCreateVertexArrays(GlShaderState::VERTEX_ARRAYS_COUNT, shaderState.vertexArrayHandles);

    cameraState.position = glm::vec3 {0, 0, -10};
    cameraState.rotation = glm::quat {glm::vec3 {0}};
}

void GlRenderSystem::update(
    entt::registry& ecsRegistry,
    GlShaderState& shaderState,
    const RendererState& rendererState,
    const CameraState& cameraState,
    const WindowState& windowState)
{
    auto& vaoHandle = shaderState.vertexArrayHandles[0];
    auto viewMatrix = transformMatrix(cameraState.position, cameraState.rotation);

    auto halfWidth = windowState.width * 0.5f;
    auto halfHeight = windowState.height * 0.5f;
    auto projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.f);

    glViewport(0, 0, windowState.width, windowState.height);
    glClearBufferfv(GL_COLOR, 0, reinterpret_cast<const GLfloat*>(&rendererState.backgroundColor));

    auto meshComponentsView = ecsRegistry.view<Mesh, GlMesh, const Position, const Rotation, const Scale>();
    for (auto&& [e, mesh, glMesh, pos, rot, scale]: meshComponentsView.each())
    {
        auto modelMatrix = transformMatrix(pos.value, rot.value, scale.value);

        glBindVertexArray(vaoHandle);

        applyVertexBuffer(glMesh, mesh);
        applyTransformMatrix(modelMatrix, viewMatrix, projectionMatrix);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.vertices.size()));
    }
}

static inline void applyVertexBuffer(const GlMesh& glMesh, const Mesh& mesh)
{
    const auto& vertexVboHandle = glMesh.vboHandle;
    const auto& vertices = mesh.vertices;
    const auto* verticesPtr = vertices.data();
    const auto verticesCount = vertices.size();
    const auto verticesBytes = static_cast<GLsizeiptr>(verticesCount * sizeof(Mesh::Vertex));

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

    glVertexAttribPointer(GlMesh::VERTEX_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), nullptr);
    glEnableVertexAttribArray(GlMesh::VERTEX_ATTRIB_INDEX);
}

static inline void applyTransformMatrix(
    const glm::mat4& modelMatrix,
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix)
{
    auto transformMatrix = projectionMatrix * viewMatrix * modelMatrix;

    glVertexAttribPointer(GlMesh::MATRIX_ATTRIB_INDEX, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(GlMesh::MATRIX_ATTRIB_INDEX + 1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(GlMesh::MATRIX_ATTRIB_INDEX + 2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(GlMesh::MATRIX_ATTRIB_INDEX + 3, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glVertexAttrib4fv(GlMesh::MATRIX_ATTRIB_INDEX, &transformMatrix[0][0]);
    glVertexAttrib4fv(GlMesh::MATRIX_ATTRIB_INDEX + 1, &transformMatrix[1][0]);
    glVertexAttrib4fv(GlMesh::MATRIX_ATTRIB_INDEX + 2, &transformMatrix[2][0]);
    glVertexAttrib4fv(GlMesh::MATRIX_ATTRIB_INDEX + 3, &transformMatrix[3][0]);
}
