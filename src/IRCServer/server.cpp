#include "server.h"
#include "../key.h"
#include <string>
#include <sstream>

void runPassCmd(IRCCommand command, int fd);
void runNickCmd(IRCCommand command, int fd, std::string &nick);
void runUserCmd(IRCCommand command, int fd);
void runQuitCmd(IRCCommand command, int fd);
void runJoinCmd(IRCCommand command, int fd, std::vector<std::string> &joined);
void runPartCmd(IRCCommand command, int fd, std::vector<std::string> &joined);
void runPrivMsgCmd(IRCCommand command, int fd);
void IRCServer::handleConnection(int newFD)
{
    bool authed = false;
    std::string nick;
    std::vector<std::string> joined;
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
            case QUIT:
            {
                runQuitCmd(parsedCommand, newFD);
            }
            case JOIN:
            {
                runJoinCmd(parsedCommand, newFD, joined);
            }
            case PART:
            {
                runPartCmd(parsedCommand, newFD, joined);
            }
            case PRIVMSG:{
                runPrivMsgCmd(parsedCommand, newFD);
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

void runQuitCmd(IRCCommand, int fd)
{
    shutdown(fd, SHUT_WR);
    close(fd);
}

void runJoinCmd(IRCCommand command, int fd, std::vector<std::string> &joined)
{
    if (command.params.size() > 1)
    {
        //error  ERR_NEEDMOREPARAMS
    }
    else
    {
        for (auto i : command.params)
        {
            if (auto channel = channels.find(i) == channels.end())
            {
                //error ERR_NOSUCHCHANNEL
            }
            else
            {
                joined.push_back(i);
                /*If a JOIN is successful, the user is then sent the channel's topic
                (using RPL_TOPIC) and the list of users who are on the channel (using
                RPL_NAMREPLY), which must include the user joining.*/
            }
        }
    }
}
void runPartCmd(IRCCommand command, int fd, std::vector<std::string> &joined)
{
    if (command.params.size() > 1)
    {
        //error  ERR_NEEDMOREPARAMS
    }
    else
    {
        for (auto i : command.params)
        {
            bool found = false;
            for (int k = 0; k < joined.size(); k++)
            {
                if (joined[k] == i)
                {
                    joined.erase(joined.begin() + k);
                }
            }
        }
    }
}

void runPrivMsgCmd(IRCCommand command, int fd){
    if (command.params.end()[0] != ':'){
        //ERR_NOTEXTTOSEND
    }
    if (command.params.size() < 2){
        //ERR_NORECIPIENT
    }
}