#include "gl_render_gui_system.h"

#include "../../../utils/debug_utils.h"
#include "../../../utils/math.h"
#include "../../components/gui_mesh.h"
#include "../../components/gl_mesh.h"
#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"

static void applyVertexBuffer(const GlMesh& glMesh, const GuiMesh& mesh);
void applyModelViewProjectionMatrix(const GlMesh& glMesh, const GuiMesh& mesh);

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
    const auto& projectionMatrix = glm::ortho(0.f, (float) windowState.width, 0.f, (float) windowState.height);
    const auto& renderComponentsView = guiRegistry.view<GuiMesh, GlMesh, const Position, const Rotation, const Scale>();

    for (auto&& [e, mesh, glMesh, pos, rot, scale]: renderComponentsView.each())
    {
        const auto vaoHandle = glMesh.vaoHandle;
        const auto& vertexVboHandle = glMesh.vertexVboHandle;
        const auto& vertices = mesh.vertices;
        const auto verticesSize = vertices.size();
        const auto& modelMatrix = Math::transformMatrix(pos.value, rot.value, scale.value);
        const auto& mvpMatrix = projectionMatrix * modelMatrix;
        mesh.modelViewProjection = mvpMatrix;

        glUseProgram(shaderState.programHandle);
        glBindVertexArray(vaoHandle);

        applyVertexBuffer(glMesh, mesh);
        applyModelViewProjectionMatrix(glMesh, mesh);
    }

    for (auto&& [e, mesh, glMesh, pos, rot, scale]: renderComponentsView.each())
    {
        glBindVertexArray(glMesh.vaoHandle);
        glDrawArrays((GLenum) mesh.renderMode, 0, (GLsizei) mesh.vertices.size());
    }
}

void applyVertexBuffer(const GlMesh& glMesh, const GuiMesh& mesh)
{
    const auto& vertexVboHandle = glMesh.vertexVboHandle;
    const auto& vertices = mesh.vertices;
    const auto* newBufferPtr = vertices.data();
    const auto newBufferSize = vertices.size() * sizeof(GuiVertex);

    glBindBuffer(GL_ARRAY_BUFFER, vertexVboHandle);

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
}

void applyModelViewProjectionMatrix(const GlMesh& glMesh, const GuiMesh& mesh)
{
    const auto rows = glm::mat4::row_type::length();
    const auto columns = glm::mat4::col_type::length();

    glBindBuffer(GL_ARRAY_BUFFER, glMesh.modelViewProjectionVboHandle);
    glNamedBufferSubData(glMesh.modelViewProjectionVboHandle, 0, sizeof(glm::mat4), &mesh.modelViewProjection);

    for (size_t i = 0; i < rows; i++)
    {
        const auto attribId = GlMesh::MVP_MATRIX_UNIFORM_INDEX + i;
        glEnableVertexAttribArray(attribId);
        glVertexAttribPointer(
            attribId, columns, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (i * columns * sizeof(float)));
        glVertexAttribDivisor(attribId, mesh.vertices.size());
    }
}
