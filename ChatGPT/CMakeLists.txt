cmake_minimum_required(VERSION 3.8.0)
project(ChatGPTDll VERSION 0.1.0)
# set to use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# set to use static runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(SOURCES src/ChatGPT.cpp src/Error.cpp)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
add_library(ChatGPTDll STATIC ${SOURCES})
target_link_libraries(ChatGPTDll PRIVATE nlohmann_json::nlohmann_json cpr::cpr)