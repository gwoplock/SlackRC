#ifndef KEY_H_
#define KEY_H_

#include "channel.h"
#include "user.h"
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <string>
#include "IRCServer/server.h"

extern std::string key;
extern std::map<std::string, Channel> channels;
extern std::map<std::string, User> users;
extern boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> *ws;
extern int currentID;
extern IRCServer server;
#endif