#include "channelList.h"

#include <iostream>
#include <string>
#include <boost/property_tree/json_parser.hpp>

std::map<std::string, Channel> parseChannels(MemoryStruct channelList)
{
    std::map<std::string, Channel> toRet;
    std::stringstream channelListSS;
    channelListSS << channelList.memory;
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(channelListSS, pt);

    for (auto &array_element : pt)
    {
        for (auto &property : array_element.second)
        {
            toRet.emplace(property.second.get<std::string>("id"), Channel(property.second));
        }
    }
    return toRet;
}

std::map<std::string, User> parseUsers(MemoryStruct userList)
{
     std::map<std::string, User> toRet;
    std::stringstream userListSS;
    userListSS << userList.memory;
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(userListSS, pt);
    for (auto &array_element : pt)
    {
        for (auto &property : array_element.second)
        {
            toRet.emplace(property.second.get<std::string>("id"), User(property.second));
        }
    }
    return toRet;
}