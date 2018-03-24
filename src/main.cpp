#include "root_certs.h"
#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/network/uri.hpp>
#include <boost/network/uri/uri_io.hpp>
#include <boost/network/uri/uri.ipp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/websocket/ssl.hpp>

//NOTE: copied from "https://curl.haxx.se/libcurl/c/getinmemory.html"

struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

//END OF COPIED CODE

//NOTE: copied from "http://www.boost.org/doc/libs/develop/libs/beast/example/websocket/client/sync-ssl/websocket_client_sync_ssl.cpp"

using tcp = boost::asio::ip::tcp;              // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket; // from <boost/beast/websocket.hpp>
namespace ssl = boost::asio::ssl;              // from <boost/asio/ssl.hpp>
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

MemoryStruct rtmConnect(std::string key)
{
    std::string url;
    url = "https://www.slack.com/api/rtm.connect?token=" + key;

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "error initing curl" << std::endl;
        exit(1);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //NOTE: copied from "https://curl.haxx.se/libcurl/c/getinmemory.html"

    MemoryStruct chunk;

    chunk.memory = (char *)malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;                   /* no data at this point */

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    /* some servers don't like requests that are made without a user-agent
		 field, so we provide one */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    //END OF COPIED CODE

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        std::cerr << "error running curl " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }
    std::cout << "size receved: " << chunk.size << std::endl;
    std::cout << "got: " << chunk.memory << std::endl;
    return chunk;
}

std::string getRTMURL(std::stringstream *jsonSS)
{
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(*jsonSS, pt);
    std::string webhookURL = pt.get<std::string>("url");
    std::cout << "url is: " << webhookURL << std::endl;
    return webhookURL;
}

auto rtmStreamConnect(std::string webhookURL)
{
    boost::network::uri::uri webhookURI(webhookURL);

    //open webhook url from res

    //NOTE: taken from http://www.boost.org/doc/libs/develop/libs/beast/example/websocket/client/sync-ssl/websocket_client_sync_ssl.cpp

    // The io_context is required for all I/O
    boost::asio::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::sslv23_client};

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // These objects perform our I/O
    tcp::resolver resolver{ioc};
    websocket::stream<ssl::stream<tcp::socket>> *ws = new websocket::stream<ssl::stream<tcp::socket>>{ioc, ctx};

    // Look up the domain name
    auto const results = resolver.resolve(webhookURI.host(), "443");

    // Make the connection on the IP address we get from a lookup
    boost::asio::connect(ws->next_layer().next_layer(), results.begin(), results.end());

    // Perform the SSL handshake
    ws->next_layer().handshake(ssl::stream_base::client);

    // Perform the websocket handshake
    std::string fullPath = webhookURI.path() + "?" + webhookURI.query();
    std::cout << "full path is: " << fullPath << std::endl;
    ws->handshake(webhookURI.host(), fullPath);
    return ws;
}

void readMessage(websocket::stream<ssl::stream<tcp::socket>> *ws)
{
    boost::beast::multi_buffer b;
    // Read a message into our buffer
    ws->read(b);
    std::stringstream message;
    message << boost::beast::buffers(b.data());
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(message, pt);
    if (pt.get<std::string>("type") == "message")
    {
        std::cout << "new message: " << pt.get<std::string>("text") << " by: " << pt.get<std::string>("user") << " in: " << pt.get<std::string>("channel") << std::endl;
    }
    if (pt.get<std::string>("type") == "hello")
    {
        std::cout << "hello receved" << std::endl;
    }
}

int main()
{
    try
    {
        //read key from file
        std::string key = getKey();
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
    //print messages as the come
}