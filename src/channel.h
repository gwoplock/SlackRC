#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <boost/property_tree/json_parser.hpp>
#include <string>
#include "user.h"

class Channel {
    private:
    std::string _name;
    boost::property_tree::ptree _pt;
    User *_users;
    int _userCount;
    public:
        Channel(){}
        Channel(boost::property_tree::ptree pt);
        std::string name(){
            return _name;
        }
        std::string topic(){
            auto temp = _pt.get_optional<std::string>("topic.value");
            if (temp){
                return *temp;
            }
        }
};

#endif