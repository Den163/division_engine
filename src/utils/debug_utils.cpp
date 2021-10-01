#include "debug_utils.h"

#include <glad/gl.h>

void DebugUtils::errorCallback(int error, const char* description)
{
    std::cerr << "Error with code: " << error << "\nDescription: " << description << std::endl;
}

void DebugUtils::printRendererInfo()
{
    const auto* renderer = glGetString(GL_RENDERER);
    const auto* vendor = glGetString(GL_VENDOR);
    const auto* version = glGetString(GL_VERSION);
    const auto* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    std::cout << "OpenGL Info: " << std::endl;
    std::cout << "GL Vendor:                " << vendor << std::endl;
    std::cout << "GL Renderer:              " << renderer << std::endl;
    std::cout << "GL Version (string):      " << version << std::endl;
    std::cout << "GL Version (integer):     " << major << "." << minor << std::endl;
    std::cout << "GLSL Version (integer):   " << glslVersion << std::endl;

    GLint extensionsCount;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsCount);

    std::cout << std::endl << "Available OpenGL extensions: " << std::endl;

    for (size_t i = 0; i < extensionsCount; i++)
    {
        std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;
    }

    std::cout << "------------------------------" << std::endl;
}
