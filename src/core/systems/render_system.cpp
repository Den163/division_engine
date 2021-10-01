#include "render_system.h"

#include <glad/gl.h>
#include <iostream>

#include "../components/gl_shader_program.h"
#include "../components/window_config.h"

#include "../components/renderer_config.h"

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

void RenderSystem::init(singleton_registry& engineData)
{
    const auto& windowCfg = engineData.get<WindowConfig>();
//    glViewport(0, 0, windowCfg.width, windowCfg.height);

    auto& shaderProgram = engineData.get<GLShaderProgram>();
    glGenBuffers(GLShaderProgram::VBO_COUNT, shaderProgram.vboHandles);
}

void RenderSystem::update(singleton_registry& engineData)
{
    auto [ shaderProgram, rendererConfig ] = engineData.get<GLShaderProgram, const RendererConfig>();

    auto& vaoHandle = shaderProgram.vaoHandle;
    auto positionsVbo = shaderProgram.vboHandles[GLShaderProgram::POSITIONS_VBO_INDEX];
    auto colorsVbo = shaderProgram.vboHandles[GLShaderProgram::COLORS_VBO_INDEX];

    glBindBuffer(GL_ARRAY_BUFFER, positionsVbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(positions.size()), positions.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(colors.size()), colors.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(GLShaderProgram::POSITIONS_VBO_INDEX);
    glEnableVertexAttribArray(GLShaderProgram::COLORS_VBO_INDEX);

    glBindBuffer(GL_ARRAY_BUFFER, positionsVbo);
    glVertexAttribPointer(GLShaderProgram::POSITIONS_VBO_INDEX, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
    glVertexAttribPointer(GLShaderProgram::COLORS_VBO_INDEX, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(vaoHandle);

    glClearBufferfv(GL_COLOR, 0, &rendererConfig.backgroundColor.r);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RenderSystem::cleanup(singleton_registry& engineData)
{

}
