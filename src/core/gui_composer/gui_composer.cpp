#include "gui_composer.h"

#include "../events/gui_mesh_created.h"

entt::entity GuiComposer::makeGuiElement(const GuiMaterial& material, const Transform& transform)
{
    const auto e = guiRegistry().create();
    {
        auto& pos = guiRegistry().emplace<Position>(e);
        pos.value = transform.position;
    }
    {
        auto& rot = guiRegistry().emplace<Rotation>(e);
        rot.value = transform.rotation;
    }
    {
        auto& scale = guiRegistry().emplace<Scale>(e);
        scale.value = transform.scale;
    }
    {
        auto& mat = guiRegistry().emplace<GuiMaterial>(e);
        mat = material;
    }
    {
        guiRegistry().emplace<GuiMeshCreated>(e);
    }

    return e;
}

entt::entity GuiComposer::makeGuiText(const GuiText& guiText, const Transform& transform)
{
    return makeGuiText(guiText, defaultFontMaterial(), transform);
}

entt::entity GuiComposer::makeGuiText(
    const GuiText& guiText,
    const GuiMaterial& material,
    const Transform& transform)
{
    const auto e = makeGuiElement(material, transform);
    {
        auto& text = guiRegistry().emplace<GuiText>(e);
        text = guiText;
    }
    {
        guiRegistry().emplace<GlTexture>(e);
    }

    return e;
}

entt::entity GuiComposer::makeGuiRect(const GuiRect& guiRect, const Transform& transform)
{
    return makeGuiRect(guiRect, defaultColoredMaterial(), transform);
}

entt::entity GuiComposer::makeGuiRect(const GuiRect& guiRect, const GuiMaterial& material, const Transform& transform)
{
    const auto e = makeGuiElement(material, transform);
    {
        auto& r = guiRegistry().emplace<GuiRect>(e);
        r = guiRect;
    }

    return e;
}

GuiMaterial GuiComposer::defaultColoredMaterial() const
{
    const auto& defaultShaders = engineState.defaultShader;
    return GuiMaterial
        {
            .shaderPipelineHandle = defaultShaders.pipeline.handle,
            .vertexShaderProgramHandle = defaultShaders.vertex().programHandle,
            .fragmentShaderProgramHandle = defaultShaders.coloredFragment().programHandle
        };
}

GuiMaterial GuiComposer::defaultTextureMaterial() const
{
    const auto& defaultShaders = engineState.defaultShader;
    return GuiMaterial
    {
        .shaderPipelineHandle = defaultShaders.pipeline.handle,
        .vertexShaderProgramHandle = defaultShaders.vertex().programHandle,
        .fragmentShaderProgramHandle = defaultShaders.textureFragment().programHandle
    };
}

GuiMaterial GuiComposer::defaultFontMaterial() const
{
    const auto& defaultShaders = engineState.defaultShader;
    return GuiMaterial
    {
        .shaderPipelineHandle = defaultShaders.pipeline.handle,
        .vertexShaderProgramHandle = defaultShaders.vertex().programHandle,
        .fragmentShaderProgramHandle = defaultShaders.fontFragment().programHandle
    };
}
