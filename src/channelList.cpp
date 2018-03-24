#include "channelList.h"

#include <iostream>
#include <string>
#include <boost/property_tree/json_parser.hpp>


std::vector<Channel> parseChannels(MemoryStruct channelList)
{
    std::vector<Channel> toRet;
    std::stringstream channelListSS;
    channelListSS << channelList.memory;
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(channelListSS, pt);

    for (auto &array_element : pt)
    {
        for (auto &property : array_element.second)
        {
             std::cout << property.second.get<std::string>("name")<< std::endl;
             toRet.push_back(Channel(property.second));
        }
    }
    return toRet;
}