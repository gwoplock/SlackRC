#include "channel.h"
#include "key.h"

#include <iostream>

Channel::Channel(boost::property_tree::ptree pt)
{
    
    _pt = pt;
    _name = pt.get<std::string>("name");
    _userCount = pt.get<int>("num_members");
    _users = new User[_userCount];
    int i = 0;
    for (auto &array_element : pt)
    {
        for (auto &property : array_element.second)
        {

            if (property.first == "")
            {
                _users[i] = users[property.second.get_value<std::string>()];
                i++;

            }
        }
    }
    for (int i =0; i < _userCount; i++) {
        std::cout << _users[i].name() << ", ";
    }
    std::cout<<std::endl;
}