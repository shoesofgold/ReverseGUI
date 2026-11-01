#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    std::cout << std::filesystem::path(argv[0]).filename() << '\n';
    std::cout << std::filesystem::absolute(argv[0]) << '\n';
}
