#include "packager.h"
#include <iostream>

int main(int argc, char *argv[])
{

    if(argc >= 2)
    {
        dataCompress(argv[1], argv[2]);
        std::cout << "Compressing data in " << argv[1] << " into " << argv[2] << "\r\n";
    }
    return 0;
}
