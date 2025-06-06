#include "packager.h"
#include <iostream>

using namespace Packager;

int main(const int argc, const char* argv[])
{
    if(argc >= 2)
    {
        std::cout << "Compressing data in " << argv[1] << " into " << argv[2] << "\r\n";
        createPackage(argv[1], argv[2]);
    }
    return 0;
}
