#include <iostream>
#include <iterator>

int main(int argc, char **argv)
{
    std::cerr << "Version: " << PSC_PROJECT_VERSION << std::endl;
    std::cerr << "OS: " << PSC_OS << std::endl;
    std::cerr << PSC_PROJECT_NAME << " - " PSC_PROJECT_VERSION << std::endl;

    return 0;
}
