
#  ChatGPT-Cpp
A header-only version of ChatGPT Official API written in C++

* [Run with Docker](#run-with-docker)
* [Development](#development)

## Run with Docker

You can run this project with Docker. This is the easiest way to get started.

### Steps

1. Clone this repository:
```bash
git clone https://github.com/jschang19/ntu-im-final-project.git
cd ntu-im-final-project
```

2. Get your OpenAI token from [here](https://platform.openai.com/) and set it as the `ENV` in the `Dockerfile` file.

3. Install [Docker](https://www.docker.com/products/docker-desktop/), then run the following commands:

```bash
docker build -t chatcpp . # this will take a while
docker run -it --env OPENAI_API_KEY=<YOUR_OPENAI_API_KEY> chatcpp
```

## Development

To develop this project, you need to install [CMake](https://cmake.org/download/) and [Vcpkg](https://github.com/microsoft/vcpkg).

1. Export your OpenAI token to environment variable
```bash
export OPENAI_API_KEY=<YOUR_OPENAI_API_KEY>
```

2. Install dependencies with VCPKG
 ```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
# Windows
./bootstrap-vcpkg.bat
# Mac/Linux
./bootstrap-vcpkg.sh
./vcpkg install nlohmann-json
./vcpkg install cpr
./vcpkg integrate install
```  

3. Build the project with CMake
```bash
rm -rf build
mkdir build
cmake -B build/ -S . -DCMAKE_TOOLCHAIN_FILE=<YOUR_VCPKG_PATH/scripts/buildsystems/vcpkg.cmake>
cd build
cmake --build .
```  

4. Run the project
```bash
./ChatGPT
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

## References
- This repository is based on [deni2312/ChatGPT-Cpp](https://github.com/deni2312/ChatGPT-Cpp)
- Dockerize this C++ project with this [article](https://medium.com/codex/a-practical-guide-to-containerize-your-c-application-with-docker-50abb197f6d4)