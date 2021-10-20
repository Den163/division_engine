#pragma once

#include <cstdint>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <vector>

#include "vertex_buffer_query.h"

struct ShaderQuery
{
    int32_t programHandle;
    uint32_t vaoHandle;

    ShaderQuery(int32_t programHandle, uint32_t vaoHandle) : programHandle(programHandle), vaoHandle(vaoHandle)
    {
        glUseProgram(programHandle);
        glBindVertexArray(vaoHandle);
    }

    inline const auto& setUniform(int32_t uniformId, const glm::mat4& value) const
    {
        glUniformMatrix4fv(uniformId, 1, GL_FALSE, &value[0][0]);
        return *this;
    }

    template<class TBufferObject>
    inline auto createEmptyBuffer(const std::vector<TBufferObject>& bufferObjects)
    {
        return VertexBufferQueryFactory::createEmptyBuffer(bufferObjects);
    }

    template<class TBufferObject>
    inline auto modifyBuffer(uint32_t vboHandle, const std::vector<TBufferObject>& bufferObjects)
    {
        return VertexBufferQueryFactory::modifyBuffer(vboHandle, bufferObjects);
    }

    inline const auto& deleteBuffer(uint32_t vboHandle)
    {
        glDeleteBuffers(1, &vboHandle);
        return *this;
    }
};