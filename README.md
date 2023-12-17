
#  ChatGPT-Cpp
A header-only version of ChatGPT Official API written in C++

* [Run with Docker](#run-with-docker)
* [Development](#development)

## Run with Docker

You can run this project with Docker. This is the easiest way to get started. You need to get OpenAI API token first. You can get it from [here](https://platform.openai.com/).

### Steps

1. Clone this repository:
```bash
git clone https://github.com/jschang19/ntu-im-final-project.git
cd ntu-im-final-project
```

2. Install [Docker](https://www.docker.com/products/docker-desktop/), then run the following commands:

```bash
docker build -t chatcpp . // this will take a while
docker run -it chatcpp
```

## Development

To develop this project, you need to install [CMake](https://cmake.org/download/) and [Vcpkg](https://github.com/microsoft/vcpkg).

1. Install dependencies with VCPKG
 ```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
// Windows
./bootstrap-vcpkg.bat
// Mac/Linux
./bootstrap-vcpkg.sh
./vcpkg install nlohmann-json
./vcpkg install cpr
./vcpkg integrate install
```  

* With CMake:
```bash
rm -rf build
mkdir build
cmake -B build/ -S . -DCMAKE_TOOLCHAIN_FILE=<YOUR_VCPKG_PATH/scripts/buildsystems/vcpkg.cmake>
cd build
cmake --build .
./ChatBot <OPENA_AI_TOKEN>
```  

Every time you want to rebuild the project, you need to run `cmake --build .` in the build directory.

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
