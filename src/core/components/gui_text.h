#pragma once


#include <data_structures/versioned_sparse_set.h>
#include <glm/vec4.hpp>

struct GuiText
{
    glm::vec4 color;
    std::string text;
    VersionedIndex font;
    uint32_t fontHeight;
};