FROM conanio/gcc11-ubuntu16.04:latest

WORKDIR /workspaces/sync-fy

COPY . /workspaces/sync-fy

USER root

RUN conan profile detect

RUN apt-get update && apt-get install -y gdb libjsoncpp-dev

RUN apt-get remove -y cmake

RUN pip3 install cmake

# for release version
# RUN conan install . --build=missing

RUN conan install . -s build_type=Debug --build=missing

RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./build/Release/generators/conan_toolchain.cmake -S. -B./build/Release -G "Unix Makefiles"

RUN cmake --build ./build/Release --target all

CMD ["/bin/bash"]