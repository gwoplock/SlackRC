#ifndef CHANNEL_LIST_H_
#define CHANNEL_LIST_H_

#include "webAPI.h"
#include "channel.h"

#include <vector>


std::vector<Channel> parseChannels(MemoryStruct channelList);
#endif