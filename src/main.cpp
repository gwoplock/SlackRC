#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <boost/beast.hpp>
#include <boost/property_tree/json_parser.hpp>

//NOTE: copied from "https://curl.haxx.se/libcurl/c/getinmemory.html"
struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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

//END OF COPIED CODE

int main()
{
    //read key from file
    std::ifstream keyFile;
    //todo take file from cmd line
    keyFile.open("SlackToken");
    if (!keyFile.is_open())
    {
        std::cerr << "error opening file" << std::endl;
        exit(1);
    }
    std::string key;
    keyFile >> key;
    std::cout << "they key read is: " << key << std::endl;
    keyFile.close();
    //rtm.connect
    std::string url;
    url = "https://www.slack.com/api/rtm.connect?token=" + key;

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "error initing curl" << std::endl;
        exit(1);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //NOTE: copied from "https://curl.haxx.se/libcurl/c/getinmemory.html"

    MemoryStruct chunk;
 
    chunk.memory = (char*) malloc(1);  /* will be grown as needed by the realloc above */ 
    chunk.size = 0;    /* no data at this point */ 

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    //END OF COPIED CODE

    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK){
        std::cerr << "error running curl " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }
    std::cout << "size receved: " << chunk.size << std::endl;
    std::cout << "got: " << chunk.memory <<std::endl;
    //read res from ^
    std::stringstream jsonSS;
    jsonSS << chunk.memory;

    boost::property_tree::ptree pt;
  boost::property_tree::read_json(jsonSS, pt);
  std::cout << "url is" << pt.get<std::string>("url") << std::endl;

    //open webhook url from res
    //print messages as the come
}