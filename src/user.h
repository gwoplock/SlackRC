#ifndef USER_H_
#define USER_H_

#include <string>
#include <boost/property_tree/json_parser.hpp>
class User{
private:
    std::string _name;
    boost::property_tree::ptree _pt;
    public:
        User(){}
        User(boost::property_tree::ptree pt);
        std::string name(){
            return _name;
        }
        bool deleted(){
            return _pt.get<bool>("deleted");
        }

};

#endif