#pragma once

struct GlDrawArraysIndirectCommand
{
    GLuint count;
    GLuint primitivesCount;
    GLuint first;
    GLuint baseInstanceOffset;
};