#ifndef IRC_SERVER_SERVER_H_
#define IRC_SERVER_SERVER_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <iostream>
#include <vector>

enum IRCCommandCode{
    PASS, NICK, USER, SERVER, OPER, QUIT, SQUIT, JOIN, PART, MODE, TOPIC, NAMES, LIST, INVITE, KICK, VERSION, STATUS, LINKS, TIME, CONNECT, TRACE, ADMIN, INFO, PRIVMSG,
    NOTICE, WHO, WHOIS, WHOWAS, KILL, PING, PONG, ERROR, AWAY, REHASH, RESTART, SUMMON, USERS, WALLOPS, USERHOST, ISON, 
};

struct IRCCommand {
    std::string prefix;
    IRCCommandCode command;
    std::vector<std::string> params;
};

class IRCServer
{
  private:
    int _socketFD;
    const int _PORT = 6667;
    struct sockaddr_in _cli_addr;
    socklen_t cli_len = sizeof(_cli_addr);

  public:
    IRCServer()
    {
        _socketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (_socketFD == -1)
        {
            std::cerr << "error creating the socket" << std::endl;
        }
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(_PORT);
        if (bind(_socketFD, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            std::cerr << "error binding socket" << std::endl;
            
        }
    }
    void IRClisten()
    {
        listen(_socketFD, 5);

        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        while (1)
        {
            int newsockfd = accept(_socketFD, (struct sockaddr *)&cli_addr, &cli_len);
            if (newsockfd < 0)
            {
                printf("die 3");
            }
            handleConnection(newsockfd);
        }
        close(_socketFD);
    }
    void handleConnection(int newFD);
    IRCCommand parseIRCCommand(std::string message);
};

#endif