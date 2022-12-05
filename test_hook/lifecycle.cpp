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
#include "../src/core/events/gui_mesh_create_event.h"
#include "../src/core/gui_composer/gui_composer.h"

static inline void checkMeshCreateByKeyPress(EngineState& state);

static inline void checkPrintCameraInfoByKeyPress(EngineState& state);
static inline glm::vec3 randomPos(const glm::vec2& min, const glm::vec2& max);

void Lifecycle::init(EngineState& engineState)
{
    auto& guiRegistry = engineState.guiRegistry;
    const auto& texture = TextureUtils::loadFromFile("assets/images/img.jpg");

    auto fontIndex = engineState.resources.fonts.insert(
        FontUtils::makeFont("assets/fonts/Roboto-Black.ttf", 88));

    GuiComposer composer { engineState };

    composer.makeImageRect(
        GuiRect{
            .center = { 400, 400 },
            .extents = { 200, 200 }
        },
        texture
    );

    composer.makeGuiText(GuiText{
        .color = Color::blue,
        .text = "Hello world",
        .font = fontIndex,
        .fontHeight = 20,
    },
    Transform::makeDefault().withPosition({0, 20, 0}));

    for (size_t i = 1; i < 10; i++)
    {
        for (size_t j = 1; j < 10; j++)
        composer.makeGuiRect(GuiRect {
            .center = {i * 100, j * 100},
            .extents = {10, 10}
        }, GuiRectColor::all(Color::red));
    }
}

void Lifecycle::preRenderUpdate(EngineState& engineState)
{
    const auto& updateTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
        engineState.renderer.renderPass.deltaTime);

    auto timeStr = std::to_string(updateTime.count());
    timeStr.resize(10);

    for (auto&& [e, guiText] : engineState.guiRegistry.view<GuiText>().each())
    {
        guiText.text = "Render pass time: " + timeStr + " milliseconds";
    }
}

void Lifecycle::postRenderUpdate(EngineState& state)
{
    checkPrintCameraInfoByKeyPress(state);
    checkMeshCreateByKeyPress(state);
}

void checkMeshCreateByKeyPress(EngineState& engineState)
{
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

void checkPrintCameraInfoByKeyPress(EngineState& engineState)
{
    const auto& cameraState = engineState.camera;

    if (engineState.input.keyPressed('P'))
    {
       std::cout << "Camera position        : " << cameraState.position << std::endl
                 << "Camera rotation (euler): " << glm::degrees(glm::eulerAngles(cameraState.rotation)) << std::endl
                 << "Mouse screen position  : " << engineState.input.postRenderMousePosition << std::endl;
    }
}

void Lifecycle::cleanup(EngineState& state)
{

}