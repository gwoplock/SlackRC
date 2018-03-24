#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <boost/property_tree/json_parser.hpp>
#include <string>

class Channel {
    private:
    std::string _name;
    boost::property_tree::ptree _pt;
    public:
        Channel(){}
        Channel(boost::property_tree::ptree pt);
        std::string name(){
            return _name;
        }

};

#endif