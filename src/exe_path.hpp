
#pragma once

#include <filesystem>

namespace executable_paths
{
    // Full path to the running executable.
    std::filesystem::path executable_path();

    // Directory containing the running executable.
    std::filesystem::path executable_directory();

    // Path to a resource relative to the executable's directory.
    std::filesystem::path resource_path(const std::filesystem::path& relative_path);
}
