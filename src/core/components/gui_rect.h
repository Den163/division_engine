#pragma once

#include <glm/vec2.hpp>

struct GuiRect
{
    glm::vec2 center;
    glm::vec2 extents;

    inline glm::vec2 size() const { return extents * 2.f; }
    inline glm::vec2 bottomLeft() const { return center - extents; }
    inline glm::vec2 topLeft() const { return { center.x - extents.x, center.y + extents.y }; }
    inline glm::vec2 topRight() const { return center + extents; }
    inline glm::vec2 bottomRight() const { return { center.x + extents.x, center.y - extents.y }; }

    static inline GuiRect fromCenterSize(const glm::vec2& center, const glm::vec2& size)
    {
        return GuiRect { center, size * 0.5f };
    }

    static inline GuiRect fromLBTR(const glm::vec2& leftBottom, const glm::vec2& topRight)
    {
        const auto& halfLeftBottom = leftBottom * 0.5f;
        const auto& halfTopRight = topRight * 0.5f;

        return GuiRect
        {
            halfLeftBottom + halfTopRight,
            halfTopRight - halfLeftBottom
        };
    }
};