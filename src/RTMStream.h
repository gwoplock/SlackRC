#include <string>

#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> * rtmStreamConnect(std::string webhookURL);
void readMessage(boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> *ws);