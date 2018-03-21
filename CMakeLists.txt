cmake_minimum_required(VERSION 3.2.2)
project(SlackRC)

#curl
set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

add_executable(SlackRC src/main.cpp)

#curl linking
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(SlackRC ${CURL_LIBRARIES})