#pragma once

#include "../data_structures/versioned_sparse_set.h"

struct GuiText
{
    VersionedIndex font;
    uint32_t fontHeight;
    std::string text;
};