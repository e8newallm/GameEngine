#include <exception>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <sstream>

#include <poll.h>

#include "texture.h"
#include "spritemap.h"

extern bool gameClosing;

using commandFunc = void (*)(std::vector<std::string>);

std::map<std::string, std::unordered_map<std::string, uint64_t> (*)()> storeList
{
    {"texture", Texture::getList},
    {"spritemap", SpriteMap::getList},
};

void printStore(std::vector<std::string> parameters)
{
    if(parameters.size() >= 2 && storeList.contains(parameters[1]))
        for(std::pair<std::string, uint64_t> entry : storeList[parameters[1]]())
        {
            std::cout << "[" << entry.first << "] = " << entry.second << "\r\n";
        }
    else
    {
        std::cout << "No store registered!\r\n";
    }
}

void exit(std::vector<std::string> parameters)
{
    (void) parameters;
    gameClosing = true;
}

std::map<std::string, commandFunc> commandList
{
    {"printStore", printStore},
    {"exit", exit},
};

static void executeCommand(std::vector<std::string> input)
{
    std::string command = input[0];
    if(commandList.contains(command))
        commandList[command](input);
    else
        std::cout << "no command found!";
}

int debugConsole()
{
    pollfd pol {STDIN_FILENO, POLLIN, 0};
    std::cout << "\r\ndebug: " << std::flush;
    while(!gameClosing)
    {
        if(poll(&pol, POLLIN, 50))
        {
            std::vector<std::string> command;
            std::string data, token;
            getline(std::cin, data);

            std::istringstream stream(data);
            while (getline(stream, token, ' '))
                command.push_back(token);

            try
            {
                executeCommand(command);
            }
            catch (std::exception const& e)
            {
                std::cout << "exception caught: " << e.what() << "\r\n";
            }

            if(!gameClosing) std::cout << "\r\ndebug: " << std::flush;
        }
    }
    return 0;
}