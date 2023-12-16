
#  ChatGPT-Cpp



<p align="left">A header-only version of ChatGPT Official API written in C++

* [Getting started.](#getting-started)
* [Writing your first bot](#writing-your-first-bot)

## Getting started.

<h2> Steps </h2>

This API is tested with C++ 17, on linux g++ and on Visual Studio 2022.

* Install dependencies with VCPKG
 ```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg install nlohmann-json
vcpkg install cpr
vcpkg integrate install
```  

* Clone the repository
```bash
git clone https://github.com/deni2312/ChatGPT-Cpp.git
```

* With CMake:
```bash
rm -rf build
mkdir build
cmake -B build/ -S . -DCMAKE_TOOLCHAIN_FILE=/Users/liaohuizhong/vcpkg/scripts/buildsystems/vcpkg.cmake
cd build
cmake --build .
./ChatBot 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
```  
Change vcpkgdirectory with the relative or absolute path of vcpkg

## Update Vcpkg
Run this command to update vcpkg:
```bash
cd ~/vcpkg
git pull
rm -rf installed
./bootstrap-vcpkg.sh
./vcpkg install nlohmann-json
./vcpkg install cpr
./vcpkg integrate install
```

Then run the cmake command again. You need to `rm -rf build/` and build again.

## Writing your first bot

<h3>Ask something!</h3>

```c++
#include <iostream>
#include "ChatGPT/include/ChatGPT.h"
#include "ChatGPT/include/Error.h"

int main(int args,char** argv){
    //API token as argument
    OpenAI::ChatGPT chatGpt{argv[1]};
    try {
        //type of user and the message to ask
        auto response = chatGpt.askChatGPT("user","Give me 5 words");
        //Iterate all answers
        for(const auto& choice:response.choices){
            std::cout<<choice.message.content;
        }
    }catch(OpenAI::Error& e){
        //JSON error returned by the server
        std::cout<<e.what();
    }
    try {
        auto response = chatGpt.askWhisper("sound.mp3");
        std::cout<<response;
    }catch(OpenAI::Error& e){
        //JSON error returned by the server
        std::cout<<e.what();
    }
}

```
Feel free to send feedbacks and open issues to improve the library.
