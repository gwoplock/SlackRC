#ifndef WEB_API_H_
#define WEB_API_H_
#include <string>

struct MemoryStruct
{
    char *memory;
    size_t size;
};

MemoryStruct rtmConnect(std::string key);

std::string getRTMURL(std::stringstream *jsonSS);
MemoryStruct getProfile(std::string key, std::string userID);
MemoryStruct getChannel(std::string key, std::string channelID);
MemoryStruct getChannelList(std::string key);

#endif