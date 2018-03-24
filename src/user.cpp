#include "user.h"

User::User(boost::property_tree::ptree pt){
    _pt = pt;
    _name = pt.get<std::string>("profile.display_name_normalized");
}