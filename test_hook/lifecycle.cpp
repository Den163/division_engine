#include "lifecycle.h"

#include <iostream>
#include <random>

#include "../src/core/components/gui_mesh.h"
#include "../src/core/utils/debug_utils.h"
#include "../src/core/utils/engine_state_helper.h"
#include "../src/core/utils/texture_utils.h"
#include "../src/core/utils/font_utils.h"
#include "../src/core/utils/color.h"
#include "../src/core/components/gui_text.h"
#include "../src/core/components/gui_material.h"
#include "../src/core/components/gl_mesh.h"
#include "../src/core/events/gui_mesh_created.h"
#include "../src/core/gui_composer/gui_composer.h"

static inline void checkMeshCreateByKeyPress(GlobalState& state);

static inline void checkPrintCameraInfoByKeyPress(GlobalState& state);
static inline glm::vec3 randomPos(const glm::vec2& min, const glm::vec2& max);

void Lifecycle::init(GlobalState& state)
{
    auto& engineState = state.engineState;
    auto& guiRegistry = engineState.guiRegistry;
    auto textureHandle = TextureUtils::loadFromFile("assets/images/img.jpg").handle;

    auto fontIndex = engineState.resources.fonts.insert(
        FontUtils::makeFont("assets/fonts/Roboto-Black.ttf", { 0, 88 }));

    GuiComposer composer { engineState };

    composer.makeGuiText(GuiText{
        .color = Color::blue,
        .font = fontIndex,
        .fontHeight = 88,
        .text = "Hello world",
    },
    Transform::makeDefault().withPosition({0, 0, 0}));

    composer.makeGuiRect(GuiRect {
        .center = {400, 400},
        .extents = {200, 200}
    }, GuiRectColor::all(Color::red));
}

void Lifecycle::preRenderUpdate(GlobalState& state)
{
    auto& engineState = state.engineState;
    auto& updateTime = engineState.renderer.frameDelta.deltaTime;

    for (auto&& [e, guiText] : engineState.guiRegistry.view<GuiText>().each())
    {
        guiText.text = "Update time: " + std::to_string(updateTime.count()) + " sec";
    }

    for (auto&& [e, rot] : engineState.guiRegistry.view<Rotation>().each())
    {
        rot.value *= glm::quat { {0,0,glm::radians(1.f)} };
    }
}

void Lifecycle::postRenderUpdate(GlobalState& state)
{
    checkPrintCameraInfoByKeyPress(state);
    checkMeshCreateByKeyPress(state);
}

void checkMeshCreateByKeyPress(GlobalState& state)
{
    auto& engineState = state.engineState;

    if (!engineState.input.keyboardState.keyHasFlag('C', InputState::KEY_STATE_PRESSED)) return;

    const auto& windowState = engineState.window;
}

glm::vec3 randomPos(const glm::vec2& min, const glm::vec2& max)
{
    std::uniform_real_distribution<float> dists[2] = {
        std::uniform_real_distribution { min.x, max.x },
        std::uniform_real_distribution { min.y, max.y },
    };

    glm::vec3 v { 0.0f };

    for (size_t i = 0; i < 2; i++)
    {
        auto randomDevice = std::random_device {};
        auto randomFunctor = std::mt19937 { randomDevice() };
        v[i] = dists[i](randomFunctor);
    }

    return v;
}

void checkPrintCameraInfoByKeyPress(GlobalState& state)
{
    auto& engineState = state.engineState;
    const auto& cameraState = engineState.camera;

    if (engineState.input.keyPressed('P'))
    {
       std::cout << "Camera position        : " << cameraState.position << std::endl
                 << "Camera rotation (euler): " << glm::degrees(glm::eulerAngles(cameraState.rotation)) << std::endl
                 << "Mouse screen position  : " << engineState.input.postRenderMousePosition << std::endl;
    }
}

void Lifecycle::cleanup(GlobalState& state)
{

}