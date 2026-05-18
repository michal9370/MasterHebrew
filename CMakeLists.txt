cmake_minimum_required(VERSION 3.10)

project(MasterHebrew)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CURL_INCLUDE_DIRS})

add_executable(MasterHebrew 
    Main.cpp 
    Syntax.cpp 
    Dictionary.cpp
    EndWordObject.cpp  
ValueObject.cpp     
)

target_link_libraries(MasterHebrew PRIVATE CURL::libcurl)