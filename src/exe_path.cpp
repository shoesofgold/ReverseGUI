
#include "exe_path.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
    #include <Windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <cstdint>
#elif defined(__linux__)
    // Uses /proc/self/exe; no additional header is needed.
#else
    #error "executable_paths: unsupported platform"
#endif

namespace executable_paths
{
    std::filesystem::path executable_path()
    {
#ifdef _WIN32
        std::vector<wchar_t> buffer(32768);

        const DWORD length = GetModuleFileNameW(
            nullptr,
            buffer.data(),
            static_cast<DWORD>(buffer.size()));

        if (length == 0 || length >= buffer.size())
            throw std::runtime_error("Could not get executable path");

        return std::filesystem::path(std::wstring(buffer.data(), length));

#elif defined(__APPLE__)
        std::uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);

        std::vector<char> buffer(size);
        if (_NSGetExecutablePath(buffer.data(), &size) != 0)
            throw std::runtime_error("Could not get executable path");

        return std::filesystem::canonical(buffer.data());

#elif defined(__linux__)
        return std::filesystem::read_symlink("/proc/self/exe");
#endif
    }

    std::filesystem::path executable_directory()
    {
        return executable_path().parent_path();
    }

    std::filesystem::path resource_path(
        const std::filesystem::path& relative_path)
    {
        return executable_directory() / relative_path;
    }
}
