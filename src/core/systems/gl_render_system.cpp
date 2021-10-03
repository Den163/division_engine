#include "gl_render_system.h"

#include <glad/gl.h>
#include <iostream>
#include <vector>

static const std::vector<float> positions {
    -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
    0.0f, 0.8f, 0.0f
};

static const std::vector<float> colors {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

void GlRenderSystem::init(GlShaderState& shaderProgram, const WindowConfig& windowConfig)
{
//    glViewport(0, 0, windowCfg.width, windowCfg.height);

    glGenBuffers(GlShaderState::VBO_COUNT, shaderProgram.vboHandles);
}

void GlRenderSystem::update(GlShaderState& shaderProgram, const RendererConfig& rendererConfig)
{
    auto& vaoHandle = shaderProgram.vaoHandle;
    auto positionsVbo = shaderProgram.vboHandles[GlShaderState::POSITIONS_VBO_INDEX];
    auto colorsVbo = shaderProgram.vboHandles[GlShaderState::COLORS_VBO_INDEX];

    glBindBuffer(GL_ARRAY_BUFFER, positionsVbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(positions.size()), positions.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(colors.size()), colors.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(GlShaderState::POSITIONS_VBO_INDEX);
    glEnableVertexAttribArray(GlShaderState::COLORS_VBO_INDEX);

    glBindBuffer(GL_ARRAY_BUFFER, positionsVbo);
    glVertexAttribPointer(GlShaderState::POSITIONS_VBO_INDEX, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
    glVertexAttribPointer(GlShaderState::COLORS_VBO_INDEX, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(vaoHandle);

    glClearBufferfv(GL_COLOR, 0, &rendererConfig.backgroundColor.r);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}