cmake_minimum_required(VERSION 3.2.2)
project(SlackRC)
set (CMAKE_CXX_STANDARD 11)

#curl
set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

#boost stuffs
FIND_PACKAGE( Boost 1.66.0 COMPONENTS system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(SlackRC src/main.cpp)

#curl linking
include_directories(${CURL_INCLUDE_DIR})


target_link_libraries(SlackRC ${CURL_LIBRARIES} ${Boost_LIBRARIES})
