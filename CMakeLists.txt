cmake_minimum_required(VERSION 3.2.2)
project(SlackRC)
set (CMAKE_CXX_STANDARD 14)

#use better compiler 

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)
#curl
set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

# Search OpenSSL
set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

#boost stuffs
FIND_PACKAGE( Boost 1.60.0 COMPONENTS system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


add_executable(SlackRC src/main.cpp src/root_certs.h src/RTMStream.cpp src/RTMStream.h src/webAPI.h src/webAPI.cpp src/key.h src/channelList.cpp src/channelList.h
src/channel.h src/channel.cpp src/user.h src/user.cpp src/IRCServer/server.h src/IRCServer/server.cpp)

#curl linking
include_directories(${CURL_INCLUDE_DIR})

target_link_libraries(SlackRC ${CURL_LIBRARIES} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
