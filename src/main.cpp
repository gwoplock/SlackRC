#include "RTMStream.h"
#include "webAPI.h"
#include "key.h"
#include "channelList.h"
#include "IRCServer/server.h"
#include <iostream>
#include <fstream>
#include <string>

//NOTE: copied from "http://www.boost.org/doc/libs/develop/libs/beast/example/websocket/client/sync-ssl/websocket_client_sync_ssl.cpp"
using tcp = boost::asio::ip::tcp;              // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket; // from <boost/beast/websocket.hpp>
namespace ssl = boost::asio::ssl;              // from <boost/asio/ssl.hpp>
//END copied

std::map<std::string, Channel> channels;
std::map<std::string, User> users;
std::string key;
websocket::stream<ssl::stream<tcp::socket>> *ws;
int currentID;
IRCServer server;

std::string getKey()
{

    std::ifstream keyFile;
    //todo take file from cmd line
    keyFile.open("SlackToken");
    if (!keyFile.is_open())
    {
        std::cerr << "error opening file" << std::endl;
        exit(1);
    }
    std::string key;
    keyFile >> key;
    //std::cout << "they key read is: " << key << std::endl;
    keyFile.close();
    return key;
}

int main()
{
    currentID = 0;

    try
    {

        //read key from file
        key = getKey();
        users = parseUsers(getUserList(key));
        channels = parseChannels(getChannelList(key));

        //rtm.connect

        MemoryStruct chunk = rtmConnect(key);

        //read res from ^

        std::stringstream *jsonSS = new std::stringstream;
        *jsonSS << chunk.memory;

        std::string webhookURL = getRTMURL(jsonSS);

        ws = rtmStreamConnect(webhookURL);
        pid_t pid = fork();
        if (pid < 0)
        {
            //error
        }
        else if (pid > 0)
        {   
            server.IRClisten();
        }
        else
        {
            while (true)
            {
                readMessage(ws);
            }
        }

        ws->close(websocket::close_code::normal);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}