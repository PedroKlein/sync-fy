FROM conanio/gcc11-ubuntu16.04:latest

WORKDIR /sync-fy

COPY . /sync-fy

USER root

RUN conan profile detect

RUN apt-get remove -y cmake

RUN pip3 install cmake

RUN conan install . --build=missing

RUN cmake -E capabilities

RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./build/Release/generators/conan_toolchain.cmake -S. -B./build/Release -G "Unix Makefiles"

RUN cmake --build ./build/Release --target all

CMD ["/bin/bash"]