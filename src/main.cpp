#include "RTMStream.h"
#include "webAPI.h"
#include "key.h"

#include <iostream>
#include <fstream>
#include <string>


std::string key;

//NOTE: copied from "http://www.boost.org/doc/libs/develop/libs/beast/example/websocket/client/sync-ssl/websocket_client_sync_ssl.cpp"
namespace websocket = boost::beast::websocket; // from <boost/beast/websocket.hpp>
//END copied

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
    std::cout << "they key read is: " << key << std::endl;
    keyFile.close();
    return key;
}


int main()
{
    try
    {
        //read key from file
        key = getKey();
        //rtm.connect

        MemoryStruct chunk = rtmConnect(key);

        //read res from ^

        std::stringstream *jsonSS = new std::stringstream;
        *jsonSS << chunk.memory;

        std::string webhookURL = getRTMURL(jsonSS);

        auto ws = rtmStreamConnect(webhookURL);

        while (true)
        {
            readMessage(ws);
        }

        ws->close(websocket::close_code::normal);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}