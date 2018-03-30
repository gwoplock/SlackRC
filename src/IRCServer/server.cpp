#include "server.h"
#include <string>
#include <sstream>

void runPassCmd(IRCCommand command, int fd);
void runNickCmd(IRCCommand command, int fd, std::string &nick);
void runUserCmd(IRCCommand command, int fd);
void IRCServer::handleConnection(int newFD)
{
    bool authed = false;
    std::string nick;
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
            if (command[strlen(command) - 1] != '\r')
            {
                //incomplete message, break out
                messageSS << command;
                break;
            }
            auto parsedCommand = parseIRCCommand(command);
            switch (parsedCommand.command)
            {
            case PASS:
            {
                runPassCmd(parsedCommand, newFD);
                break;
            }
            case NICK:
            {
                runNickCmd(parsedCommand, newFD, nick);
            }
            case USER:
            {
                runUserCmd(parsedCommand, newFD);
            }
            }
        }
    }
}

IRCCommand IRCServer::parseIRCCommand(std::string command)
{
    IRCCommand toRet;
    std::stringstream commandSS;
    commandSS << command;
    if (commandSS.peek() == ':')
    {
        commandSS >> toRet.prefix;
    }
    std::string temp;
    commandSS >> temp;
    toRet.command = stringTOEnum[temp];
    while (!commandSS.eof())
    {
        if (commandSS.peek() == ':')
        {
            toRet.params.push_back(commandSS.str());
            break;
        }
        else
        {
            commandSS >> temp;
            toRet.params.push_back(temp);
        }
    }
    return toRet;
}

void runPassCmd(IRCCommand command, int fd)
{
    if (command.params.size() != 1)
    {
        write(fd, ERR_NEEDMOREPARAMS, strlen(ERR_NEEDMOREPARAMS));
    }
}

void runNickCmd(IRCCommand command, int fd, std::string &nick)
{
    if (command.params.size() != 1 || command.params.size() != 2)
    {
        write(fd, ERR_NONICKNAMEGIVEN, strlen(ERR_NONICKNAMEGIVEN));
    }
    else
    {
        nick = command.params.at(0);
    }
}
void runUserCmd(IRCCommand command, int fd)
{
    //i dont think i need this for what im doing
}