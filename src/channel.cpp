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
                auto temp = users[property.second.get_value<std::string>()];
                if (!temp.deleted()){
                    _users[i] = temp;
                    i++;
                }

            }
        }
    }
}