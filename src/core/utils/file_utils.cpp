#pragma once

#include <fstream>
#include "file_utils.h"

std::vector<uint8_t> readBytes(const std::string& filePath)
{
    std::ifstream file {
        filePath,
        std::ios::in | std::ios::binary | std::ios::ate
    };
    if (!file.good()) throw std::runtime_error {"Failed to open " + filePath};

    std::vector<uint8_t> bytes(file.tellg());
    file.seekg(0, std::ifstream::beg);
    if (!file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size())))
    {
        throw std::runtime_error { "Failed to read file " + filePath };
    }

    return bytes;
}
