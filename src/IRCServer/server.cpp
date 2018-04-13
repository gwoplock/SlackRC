#include "server.h"
#include "../key.h"
#include <string>
#include <sstream>

std::map<std::string, IRCCommandCode> stringTOEnum = {{"pass", PASS},
                                                      {"NICK", NICK},
                                                      {"USER", USER},
                                                      {"SERVER", SERVER},
                                                      {"OPER", OPER},
                                                      {"QUIT", QUIT},
                                                      {"SQUIT", SQUIT},
                                                      {"JOIN", JOIN},
                                                      {"PART", PART},
                                                      {"MODE", MODE},
                                                      {"TOPIC", TOPIC},
                                                      {"NAMES", NAMES},
                                                      {"LIST", LIST},
                                                      {"INVITE", INVITE},
                                                      {"KICK", KICK},
                                                      {"VERSION", VERSION},
                                                      {"STATUS", STATUS},
                                                      {"LINKS", LINKS},
                                                      {
                                                          "TIME",
                                                          TIME,
                                                      },
                                                      {"CONNECT", CONNECT},
                                                      {"TRACE", TRACE},
                                                      {"ADMIN", ADMIN},
                                                      {"INFO", INFO},
                                                      {"PRIVMSG", PRIVMSG},
                                                      {"NOTICE", NOTICE},
                                                      {"WHO", WHO},
                                                      {"WHOIS", WHOIS},
                                                      {"WHOWAS", WHOWAS},
                                                      {"KILL", KILL},
                                                      {"PING", PING},
                                                      {"PONG", PONG},
                                                      {"ERROR", ERROR},
                                                      {"AWAY", AWAY},
                                                      {"REHASH", REHASH},
                                                      {"RESTART", RESTART},
                                                      {"SUMMON", SUMMON},
                                                      {"USERS", USERS},
                                                      {"WALLOPS", WALLOPS},
                                                      {"USERHOST", USERHOST},
                                                      {"ISON", ISON}};

void runPassCmd(IRCCommand command, int fd);
void runNickCmd(IRCCommand command, int fd, std::string &nick);
void runUserCmd(IRCCommand command, int fd);
void runQuitCmd(IRCCommand command, int fd, bool& connectionOpen);
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
        std::cout << "message = " << message << std::endl;
        messageSS << (std::string)message;
        //messageSS.clear();
        std::cout << "messageSS = " << messageSS.str() << std::endl;
        while (!messageSS.eof())
        {
            bzero(command, 513);
            std::cout << "looping messageSS = " << messageSS.str() << std::endl;
            messageSS.getline(command, 513, '\n');
            std::cout << "command = " << command << std::endl;
            if (command[strlen(command) - 1] != '\r')
            {
                //incomplete message, break out
                messageSS << command;
                break;
            }
            auto parsedCommand = parseIRCCommand(command);
            std::cout << "prefix = " << parsedCommand.prefix << " command = " << parsedCommand.command << " args = ";
            for (auto i : parsedCommand.params){
                std::cout << i << ", ";
            }
            std::cout << std::endl;
            switch (parsedCommand.command)
            {
            case PASS:
            {
                runPassCmd(parsedCommand, newFD);
                break;
            }
            case NICK:
            {
                std::cout << "in NICK" << std::endl;
                runNickCmd(parsedCommand, newFD, nick);
                break;
            }
            case USER:
            {
                std::cout << "in USER" << std::endl;
                runUserCmd(parsedCommand, newFD);
                break;
            }
            case QUIT:
            {
                runQuitCmd(parsedCommand, newFD, connectionOpen);
                break;
            }
            case JOIN:
            {
                runJoinCmd(parsedCommand, newFD, joined);
                break;
            }
            case PART:
            {
                runPartCmd(parsedCommand, newFD, joined);
                break;
            }
            case PRIVMSG:
            {
                runPrivMsgCmd(parsedCommand, newFD);
                break;
            }
            default:{
                std::cout << "hit default" << std::endl;
                break;
            }
            }
        }
        if (messageSS.eof()){
            messageSS = std::stringstream();
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
    char tempArr[513];
    commandSS >> temp;
    toRet.command = stringTOEnum[temp];
    while (!commandSS.eof())
    {
        commandSS.ignore();
        if (commandSS.peek() == ':')
        {
            std::cout << "in : block" << std::endl;
            commandSS.getline(tempArr, 512, EOF);
            std::cout << "tempAr = " << tempArr <<std::endl;
            toRet.params.push_back(tempArr);
            toRet.params.push_back(tempArr);
            break;
        }
        else
        {
            commandSS >> temp;
            toRet.params.push_back(temp);
        }
    }
    toRet.params.pop_back();
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

void runQuitCmd(IRCCommand, int fd, bool& connectionOpen)
{
    shutdown(fd, SHUT_WR);
    close(fd);
    connectionOpen = false;
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
            for (auto it = channels.begin(); it != channels.end(); ++it)
            {
                if ("#" + it->second.name() == i || "&" + it->second.name() == i)
                {
                    joined.push_back(it->first);
                }
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

void runPrivMsgCmd(IRCCommand command, int fd)
{
    std::cout << "got priv msg" << std::endl;
    std::cout << *(command.params.end() - 1) << std::endl;
    if (command.params.size() < 2)
    {
        //ERR_NORECIPIENT
        std::cout << "no recip" << std::endl;
        return;
    }
    std::stringstream output;
    for (int i = 0; i < command.params.size() - 1; i++)
    {
        std::cout << "loop out" << std::endl;
        for (auto it = channels.begin(); it != channels.end(); ++it)
        {
            std::cout << "loop in" << std::endl;
            if ("#" + it->second.name() == command.params[i] || "&" + it->second.name() == command.params[i])
            {
                std::cout << "found channel" << std::endl;
                boost::property_tree::ptree pt;
                pt.put("id", ++currentID);
                pt.put("type", "message");
                pt.put("channel", it->first);
                pt.put("text", (*(command.params.end() - 1)).c_str() + 1);
                boost::property_tree::write_json(output, pt);
                std::cout << output.str() << std::endl;
                ws->write(boost::asio::buffer(std::string(output.str())));
            }
        }
    }
}