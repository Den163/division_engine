#include "gl_gui_mesh_vertex_system.h"

#include "../../../utils/debug_utils.h"
#include "../../../utils/engine_state_helper.h"
#include "../../../utils/math.h"
#include "../../components/gui_mesh.h"
#include "../../components/gl_mesh.h"
#include "../../components/position.h"
#include "../../components/rotation.h"
#include "../../components/scale.h"

static void applyVertexBuffer(const GlMesh& glMesh, const GuiMesh& mesh);
void applyModelViewProjectionMatrix(const GlMesh& glMesh, const GuiMesh& mesh, const glm::mat4& mvpMatrix);

void GlGuiMeshVerticesSystem::init(EngineState& engineState)
{
    auto& cameraState = engineState.camera;

    glEnable(GL_DEBUG_OUTPUT);
    glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
    glDebugMessageCallback(DebugUtils::glRendererMessageCallback, nullptr);

    cameraState.position = glm::vec3 {0};
    cameraState.rotation = glm::identity<glm::quat>();
    cameraState.nearPlane = 0.1f;
    cameraState.farPlane = 100;
}

void GlGuiMeshVerticesSystem::update(EngineState& engineState)
{
    auto& guiRegistry = engineState.guiRegistry;

    const auto& windowState = engineState.window;
    const auto& projectionMatrix = glm::ortho(0.f, (float) windowState.width, 0.f, (float) windowState.height);

    for (auto&& [e, mesh, glMesh, pos, rot, scale]:
         guiRegistry.view<GuiMesh, GlMesh, const Position, const Rotation, const Scale>().each())
    {
        const auto vaoHandle = glMesh.vaoHandle;
        const auto& vertexVboHandle = glMesh.vertexVboHandle;
        const auto& vertices = mesh.vertices;
        const auto verticesSize = vertices.size();
        const auto& modelMatrix = Math::transformMatrix(pos.value, rot.value, scale.value);
        const auto& mvpMatrix = projectionMatrix * modelMatrix;
        const auto shaderPipelineHandle = EngineStateHelper::shaderPipeline(engineState, mesh.shaderPipelineIndex);

        glBindVertexArray(vaoHandle);
        
        applyVertexBuffer(glMesh, mesh);
        applyModelViewProjectionMatrix(glMesh, mesh, mvpMatrix);
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
        glNamedBufferSubData(vertexVboHandle, 0, (GLsizeiptr) newBufferSize, newBufferPtr);
    }
    else
    {
        glNamedBufferStorage(vertexVboHandle, (GLsizeiptr) newBufferSize, newBufferPtr, GL_DYNAMIC_STORAGE_BIT);
    }

    glEnableVertexAttribArray(EngineInvariants::VERTEX_POSITION_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_POSITION_ATTRIBUTE_LOCATION,
        glm::vec3::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, position)));

    glEnableVertexAttribArray(EngineInvariants::VERTEX_COLOR_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_COLOR_ATTRIBUTE_LOCATION,
        glm::vec4::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, color)));

    glEnableVertexAttribArray(EngineInvariants::VERTEX_UV_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(
        EngineInvariants::VERTEX_UV_ATTRIBUTE_LOCATION,
        glm::vec2::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(GuiVertex),
        reinterpret_cast<void*>(offsetof(GuiVertex, uv)));
}

void applyModelViewProjectionMatrix(const GlMesh& glMesh, const GuiMesh& mesh, const glm::mat4& mvpMatrix)
{
    const auto rows = glm::mat4::row_type::length();
    const auto columns = glm::mat4::col_type::length();

    glBindBuffer(GL_ARRAY_BUFFER, glMesh.modelViewProjectionVboHandle);
    glNamedBufferSubData(glMesh.modelViewProjectionVboHandle, 0, sizeof(glm::mat4), &mvpMatrix);

    for (size_t i = 0; i < rows; i++)
    {
        const auto attribId = EngineInvariants::MVP_MATRIX_ATTRIBUTE_LOCATION + i;
        glEnableVertexAttribArray(attribId);
        glVertexAttribPointer(
            attribId, columns, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (i * columns * sizeof(float)));
        glVertexAttribDivisor(attribId, mesh.vertices.size());
    }
}
