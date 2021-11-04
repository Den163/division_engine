#include "gui_composer.h"

#include "../events/gui_mesh_create_event.h"
#include "../utils/color.h"
#include "../events/gui_mesh_destroy_event.h"

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
        guiRegistry().emplace<GuiMeshCreateEvent>(e);
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

entt::entity GuiComposer::makeGuiRect(const GuiRect& guiRect, const GuiRectColor& color, const Transform& transform)
{
    return makeGuiRect(guiRect, color, defaultColoredMaterial(), transform);
}

entt::entity GuiComposer::makeGuiRect(
    const GuiRect& guiRect, const GuiRectColor& color, const GuiMaterial& material, const Transform& transform)
{
    const auto e = makeGuiElement(material, transform);
    {
        auto& r = guiRegistry().emplace<GuiRect>(e);
        r = guiRect;
    }
    {
        auto& c = guiRegistry().emplace<GuiRectColor>(e);
        c = color;
    }

    return e;
}


entt::entity GuiComposer::makeImageRect(
    const GuiRect& guiRect, const Texture2dState& textureState, const Transform& transform)
{
    return makeImageRect(guiRect, textureState, GuiRectColor::all(Color::white), defaultTextureMaterial(), transform);
}

entt::entity GuiComposer::makeImageRect(
    const GuiRect& guiRect,
    const Texture2dState& textureState,
    const GuiRectColor& color,
    const GuiMaterial& material,
    const Transform& transform)
{
    const auto e = makeGuiRect(guiRect, color, material, transform);
    {
        auto& t = guiRegistry().emplace<GlTexture>(e);
        t.handle = textureState.handle;
    }
    return e;
}

void GuiComposer::destroyGuiElement(entt::entity entity)
{
    {
        guiRegistry().emplace<GuiMeshDestroyed>(entity);
    }
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
