#include "server.h"
#include <string>
#include <sstream>


void IRCServer::handleConnection(int newFD){
    bool authed = false;
    bool connectionOpen = true;
    char buffer[513];
    std::stringstream commandSS;
    while (connectionOpen){
        bzero(buffer, 513);
        int n = read(newFD, buffer, 513);
        parseIRCCommand(buffer);
    }
}

IRCCommand IRCServer::parseIRCCommand(char message[513]){

}
