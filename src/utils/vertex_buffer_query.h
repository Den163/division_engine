#pragma once

#include <cstdint>
#include <glad/gl.h>
#include <glm/vec3.hpp>
#include <vector>

#include "memory.h"
#include "gl_utils.h"
#include "../core/configs/render_mode.h"

struct VertexBufferQueryFactory
{
    template<typename TBufferObject> struct VertexBufferQuery;

    template<typename TBufferObject>
    inline static VertexBufferQuery<TBufferObject> createEmptyBuffer(
        uint32_t& vboHandleToWrite, const std::vector<TBufferObject>& bufferObjects)
    {
        glCreateBuffers(1, &vboHandleToWrite);
        glNamedBufferStorage(
            vboHandleToWrite,
            bufferObjects.size() * sizeof(TBufferObject),
            nullptr,
            GL_DYNAMIC_STORAGE_BIT
        );

        return VertexBufferQuery<TBufferObject> { vboHandleToWrite };
    }

    template<typename TBufferObject>
    inline static VertexBufferQuery<TBufferObject> modifyBuffer(
        uint32_t bufferId, const std::vector<TBufferObject>& bufferObjects)
    {
        const auto* newBufferPtr = bufferObjects.data();
        const auto newBufferSize = bufferObjects.size() * sizeof(TBufferObject);

        glBindBuffer(GL_ARRAY_BUFFER, bufferId);

        GLint currentBufferSize;
        glGetNamedBufferParameteriv(bufferId, GL_BUFFER_SIZE, &currentBufferSize);

        if (currentBufferSize == newBufferSize)
        {
            glNamedBufferSubData(bufferId, 0, newBufferSize, newBufferPtr);
        }
        else
        {
            glNamedBufferStorage(bufferId, newBufferSize, newBufferPtr, GL_DYNAMIC_STORAGE_BIT);
        }

        return VertexBufferQuery<TBufferObject> { bufferId };
    }

    template<typename TBufferObject>
    struct VertexBufferQuery
    {
        const uint32_t vboHandle;

        template<auto VectorLength, typename TVectorType, glm::qualifier VectorQualifier>
        inline const VertexBufferQuery<TBufferObject>& bindVecAttributeToField(
            int32_t attributeId,
            glm::vec<VectorLength, TVectorType, VectorQualifier> TBufferObject::*field) const
        {
            static_assert(VectorLength >= 2 && VectorLength <= 4);

            glVertexAttribPointer(
                attributeId,
                VectorLength,
                GlUtils::mapToGlType<TVectorType>(),
                GL_FALSE,
                sizeof(TBufferObject),
                (void*) Memory::offsetOf(field));

            glEnableVertexAttribArray(attributeId);

            return *this;
        }

        inline void draw(RenderMode renderMode, const size_t bufferObjectsCount) const
        {
            glDrawArrays(static_cast<GLenum>(renderMode), 0, bufferObjectsCount);
        }
    };
};