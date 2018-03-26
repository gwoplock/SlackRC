#include "server.h"
#include <string>
#include <sstream>

void IRCServer::handleConnection(int newFD)
{
    bool authed = false;
    bool connectionOpen = true;
    char message[513];
    std::stringstream messageSS;
    char command[513];
    while (connectionOpen)
    {
        bzero(message, 513);
        int n = read(newFD, message, 513);
        messageSS << message;
        while (!messageSS.eof())
        {
            messageSS.getline(command, 513, '\n');
            if (command[strlen(command) - 1] != '\r'){
                //incomplete message, break out
                messageSS << command;
                break;
            }
            parseIRCCommand(command);

        }
    }
}

IRCCommand IRCServer::parseIRCCommand(std::string command)
{
    std::stringstream commandSS;
    
}
