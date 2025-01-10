#include "packager.h"
#include <iostream>

int main(const int argc, const char *argv[])
{
    if(argc >= 2)
    {
        std::cout << "Compressing data in " << argv[1] << " into " << argv[2] << "\r\n";
        dataCompress(argv[1], argv[2]);
    }
    return 0;
}
