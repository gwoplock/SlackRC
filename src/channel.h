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
        std::string id(){
            
        }
};

#endif