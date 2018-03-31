#include "RTMStream.h"
#include "key.h"
#include "webAPI.h"
#include "root_certs.h"

#include <boost/asio/connect.hpp>

#include <boost/property_tree/json_parser.hpp>

#include <boost/network/uri.hpp>
#include <boost/network/uri/uri.ipp>

using tcp = boost::asio::ip::tcp;              // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket; // from <boost/beast/websocket.hpp>
namespace ssl = boost::asio::ssl;              // from <boost/asio/ssl.hpp>

websocket::stream<ssl::stream<tcp::socket>> * rtmStreamConnect(std::string webhookURL)
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
    //std::cout << "full path is: " << fullPath << std::endl;
    ws->handshake(webhookURI.host(), fullPath);
    return ws;
    //END COPIED
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
        std::string text = pt.get<std::string>("text");
        std::string userID = pt.get<std::string>("user");
        std::string channelID =  pt.get<std::string>("channel");
        std::cout << "new message: " << text << " by: " << users[userID].name() << " (" << userID << ")" << " in: " 
        << channels["#" + channelID].name()   << std::endl;
    }
    if (pt.get<std::string>("type") == "hello")
    {
        std::cout << "hello receved" << std::endl;
    }
}