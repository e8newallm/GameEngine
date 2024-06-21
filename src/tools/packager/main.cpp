#include "packager.h"
#include <iostream>

int main(int argc, char *argv[])
{

    if(argc >= 2)
    {
        dataCompress(argv[1], argv[2]);
    }
    return 0;
}
