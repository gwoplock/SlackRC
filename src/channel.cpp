#include "channel.h"

Channel::Channel(boost::property_tree::ptree pt){
    _pt = pt;
    _name = pt.get<std::string>("name");
}