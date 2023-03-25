cmake_minimum_required(VERSION 3.12)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    /usr/include/nlohmann)

add_library(Config
    config.cpp
)

target_link_directories(Config PUBLIC
    "/usr/include/nlohmann"
)

set_property(TARGET Config PROPERTY CXX_STANDARD 20)