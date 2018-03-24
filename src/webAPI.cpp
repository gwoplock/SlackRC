//NOTE: copied from "https://curl.haxx.se/libcurl/c/getinmemory.html"

#include "webAPI.h"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>

#include <curl/curl.h>

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

MemoryStruct webAPIDo(std::string url)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "error initing curl" << std::endl;
        exit(1);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //NOTE: copied from "https://curl.haxx.se/libcurl/c/getinmemory.html"

    MemoryStruct chunk;

    chunk.memory = (char *)malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;                   /* no data at this point */

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    /* some servers don't like requests that are made without a user-agent
		 field, so we provide one */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    //END OF COPIED CODE

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        std::cerr << "error running curl " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }
    /*std::cout << "size receved: " << chunk.size << std::endl;
    std::cout << "got: " << chunk.memory << std::endl;*/
    return chunk;
}



MemoryStruct rtmConnect(std::string key)
{
    std::string url;
    url = "https://www.slack.com/api/rtm.connect?token=" + key;
    return webAPIDo(url);
}

std::string getRTMURL(std::stringstream *jsonSS)
{
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(*jsonSS, pt);
    std::string webhookURL = pt.get<std::string>("url");
    //std::cout << "url is: " << webhookURL << std::endl;
    return webhookURL;
}

MemoryStruct getProfile(std::string key, std::string userID)
{
    std::string url;
    url = "https://www.slack.com/api/users.profile.get?token=" + key + "&user=" + userID;
    return webAPIDo(url);
}

MemoryStruct getChannel(std::string key, std::string channelID)
{
    std::string url;
    url = "https://www.slack.com/api/channels.info?token=" + key + "&channel=" + channelID;

    return webAPIDo(url);
}

MemoryStruct getChannelList(std::string key)
{
    std::string url;
    url = "https://www.slack.com/api/channels.list?token=" + key;

    return webAPIDo(url);
}