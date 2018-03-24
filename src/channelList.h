#ifndef CHANNEL_LIST_H_
#define CHANNEL_LIST_H_

#include "webAPI.h"
#include "channel.h"

#include <map>


std::map<std::string,Channel> parseChannels(MemoryStruct channelList);
#endif