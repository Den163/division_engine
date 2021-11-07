#include "gl_mvp_matrix_system.h"

#include <components/gl_mesh.h>
#include <components/gui_mesh.h>
#include <components/position.h>
#include <components/rotation.h>
#include <components/scale.h>
#include <utils/math.h>

void GlMvpMatrixSystem::update(EngineState& engineState)
{
    const auto& windowState = engineState.window;
    const auto& projectionMatrix = glm::ortho(
        0.f, (float) windowState.frameBufferPixelsWidth,
        0.f, (float) windowState.frameBufferPixelsHeight);
    const auto& guiRegistry = engineState.guiRegistry;

    for (auto&& [e, glMesh, guiMesh, position, rotation, scale] :
         guiRegistry.view<const GlMesh, const GuiMesh, const Position, const Rotation, const Scale>().each())
    {
        const auto& modelMatrix = Math::transformMatrix(position.value, rotation.value, scale.value);
        const auto& mvpMatrix = projectionMatrix * modelMatrix;
        const auto rows = glm::mat4::row_type::length();
        const auto columns = glm::mat4::col_type::length();

        glBindVertexArray(glMesh.vaoHandle);
        glBindBuffer(GL_ARRAY_BUFFER, glMesh.modelViewProjectionVboHandle);
        glNamedBufferSubData(glMesh.modelViewProjectionVboHandle, 0, sizeof(glm::mat4), &mvpMatrix);

        for (size_t i = 0; i < rows; i++)
        {
            const auto attribId = EngineInvariants::MVP_MATRIX_ATTRIBUTE_LOCATION + i;
            glEnableVertexAttribArray(attribId);
            glVertexAttribPointer(
                attribId, columns, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (i * columns * sizeof(float)));
            glVertexAttribDivisor(attribId, guiMesh.verticesCount);
        }
    }
}