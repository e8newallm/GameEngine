#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    for(int i = 0; i < argc; i++)
    {
        std::cout << "arg " << i << ": " << argv[i] << "\r\n";
    }

    for(int i = 2; i < argc; i++)
    {
        std::string data = "";
        std::string path = argv[i];
        std::string schema = path.substr(path.find_last_of("/\\") + 1);
        schema = schema.substr(0, schema.find_last_of('.'));

        std::ifstream schemaFile(path);
        std::stringstream buffer;
        buffer << schemaFile.rdbuf();
        schemaFile.close();
        data += "const char* " + std::string(schema) + " = R\"SCHEMA(\r\n" + buffer.str() + ")SCHEMA\";\r\n\r\n";

        std::string headerFile = std::string(argv[1]) + "/" + schema + ".h";
        if(std::filesystem::exists(headerFile))
        {
            std::ifstream existHeader(headerFile);
            std::stringstream existBuffer;
            existBuffer << existHeader.rdbuf();
            existHeader.close();

            if(existBuffer.str() == data)
            {
                continue;
            }
        }

        std::fstream header(headerFile);
        
        header << data;
        header.close();
    }
}