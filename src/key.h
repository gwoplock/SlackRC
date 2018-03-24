#ifndef KEY_H_
#define KEY_H_

#include "channel.h"
#include "user.h"

#include <string>

extern std::string key;
extern std::map<std::string, Channel> channels;
extern std::map<std::string, User> users;
#endif