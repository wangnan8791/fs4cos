CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_policy(SET CMP0015 NEW)

include_directories(../include)
include_directories(/usr/local/include/fuse)
link_directories(../lib)
link_directories(/usr/local/lib)

add_definitions(-D_FILE_OFFSET_BITS=64)

ADD_EXECUTABLE(fs4cos fs4cos.cpp)
target_link_libraries(fs4cos cosdk fuse curl crypto ssl rt jsoncpp z)
