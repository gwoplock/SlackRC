#include <iostream>
#include <fstream>
#include <string>


int main(){
    //read key from file
    std::ifstream keyFile;
    //todo take file from cmd line
    keyFile.open("SlackToken");
    if (!keyFile.is_open()){
        std::cerr << "error opening file" << std::endl;
        exit(1);
    }
    std::string key;
    keyFile >> key;
    std:: cout << key;
    keyFile.close();
    //rtm.connect
    std::string url;
    url = "https://www.slack.com/api/rtm.connect?token=" + key;
    
    //read res from ^
    //open webhook url from res
    //print messages as the come
}