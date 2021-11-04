#include "gl_gui_rect_vertex_system.h"

#include "../../components/gl_mesh.h"
#include "../../components/gui_mesh.h"
#include "../../components/gui_rect.h"
#include "../../utils/color.h"
#include "../../utils/gl_utils.h"
#include "../../components/gui_rect_color.h"

void GlGuiRectVertexSystem::update(EngineState& engineState)
{
    for (auto&& [e, guiMesh, glMesh, guiRect, guiRectColor] :
         engineState.guiRegistry.view<GuiMesh, const GlMesh, const GuiRect, const GuiRectColor>().each())
    {
        guiMesh.primitivesCount = 1;
        guiMesh.verticesCount = 4;

        GlUtils::makeGuiVertexBufferStorage(glMesh, guiMesh);

        auto color = Color::white;
        glBindBuffer(GL_ARRAY_BUFFER, glMesh.vertexVboHandle);
        GuiVertex* vertexData = (GuiVertex*) glMapNamedBuffer(glMesh.vertexVboHandle, GL_WRITE_ONLY);
        vertexData[0] = GuiVertex { guiRectColor.topLeft, guiRect.topLeft(), { 0, 1 } };
        vertexData[1] = GuiVertex { guiRectColor.bottomLeft, guiRect.bottomLeft(), { 0, 0 } };
        vertexData[2] = GuiVertex { guiRectColor.topRight, guiRect.topRight(), { 1, 1 } };
        vertexData[3] = GuiVertex { guiRectColor.bottomRight, guiRect.bottomRight(), { 1, 0 } };
        glUnmapNamedBuffer(glMesh.vertexVboHandle);
    }
}
