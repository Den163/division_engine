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
    inline static VertexBufferQuery<TBufferObject> createEmptyBuffer(const std::vector<TBufferObject>& bufferObjects)
    {
        uint32_t vboHandle;
        glCreateBuffers(1, &vboHandle);
        return VertexBufferQuery<TBufferObject> { vboHandle }
            .setEmptyBuffer(bufferObjects.size() * sizeof(TBufferObject));
    }

    template<typename TBufferObject>
    inline static VertexBufferQuery<TBufferObject> modifyBuffer(
        uint32_t bufferId, const std::vector<TBufferObject>& bufferObjects)
    {
        return VertexBufferQuery<TBufferObject> { bufferId }.modifyBuffer(bufferObjects);
    }

    template<typename TBufferObject>
    struct VertexBufferQuery
    {
        const uint32_t vboId;

        explicit VertexBufferQuery(uint32_t vboId) : vboId(vboId)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vboId);
        }

        inline const VertexBufferQuery<TBufferObject>& setEmptyBuffer(size_t size) const
        {
            glNamedBufferStorage(vboId, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
            return *this;
        }

        inline const VertexBufferQuery<TBufferObject>& modifyBuffer(
            const std::vector<TBufferObject>& bufferObjects) const
        {
            const auto* newBufferPtr = bufferObjects.data();
            const auto newBufferSize = bufferObjects.size() * sizeof(TBufferObject);

            GLint currentBufferSize;
            glGetNamedBufferParameteriv(vboId, GL_BUFFER_SIZE, &currentBufferSize);

            if (currentBufferSize == newBufferSize)
            {
                glNamedBufferSubData(vboId, 0, newBufferSize, newBufferPtr);
            }
            else
            {
                glNamedBufferStorage(vboId, newBufferSize, newBufferPtr, GL_DYNAMIC_STORAGE_BIT);
            }

            return *this;
        }

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