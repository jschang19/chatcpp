# Stage 1: Build stage
FROM ubuntu:20.04 as build

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV cmake_version=3.27.7
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
ENV VCPKG_ROOT /vcpkg
ENV PATH "${VCPKG_ROOT}:${PATH}"

# Update the package list and install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    curl \
    libssl-dev \
    zip \
    unzip \
    tar \
    wget \
    pkg-config \
    ca-certificates \
    gpg \
    ninja-build

# Install a specific version of CMake
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | \
    gpg --dearmor - | \
    tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null && \
    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' | \
    tee /etc/apt/sources.list.d/kitware.list >/dev/null && \
    apt-get update && rm /usr/share/keyrings/kitware-archive-keyring.gpg && \
    apt-get -y install kitware-archive-keyring cmake && \
    rm -rf /var/lib/apt/lists/* && apt-get clean

# Clone vcpkg and install packages
RUN git clone https://github.com/Microsoft/vcpkg.git \
    && export VCPKG_FORCE_SYSTEM_BINARIES=1 \
    && cd vcpkg && ./bootstrap-vcpkg.sh && ./vcpkg integrate install
    
RUN vcpkg install cpr nlohmann-json

WORKDIR /app
COPY . .

# Build the project using CMake
RUN cmake -B build/ -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
RUN cmake --build build/

# Stage 2: Runtime stage
FROM ubuntu:20.04 as runtime

# Set necessary environment variables for runtime
ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libssl-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/* && apt-get clean

# Copy the built application from the build stage
COPY --from=build /app/build/ /app/

# Set the working directory
WORKDIR /app

# Command to run the application
CMD ["./ChatBot"]