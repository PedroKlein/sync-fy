FROM conanio/gcc11-ubuntu16.04:latest

WORKDIR /workspaces/sync-fy

COPY . /workspaces/sync-fy

USER root

RUN conan profile detect

ARG BUILD_TYPE=Debug

RUN echo "Debug: BUILD_TYPE=${BUILD_TYPE}"

RUN apt-get update && apt-get install -y gdb libjsoncpp-dev

RUN apt-get remove -y cmake

RUN pip3 install cmake

RUN conan install . -s build_type=$BUILD_TYPE --build=missing

RUN cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_TOOLCHAIN_FILE=./build/$BUILD_TYPE/generators/conan_toolchain.cmake -S. -B./build/$BUILD_TYPE -G "Unix Makefiles"

RUN cmake --build ./build/$BUILD_TYPE --target all