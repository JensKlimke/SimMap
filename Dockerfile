FROM ubuntu:18.04
MAINTAINER Jens Klimke (jens.klimke@rwth-aachen.de)

# installation
RUN apt-get update
RUN apt-get -y install git g++ make cmake nano doxygen graphviz
RUN apt-get -y install autoconf automake libtool curl make g++ unzip
RUN cmake --version && g++ --version

# install protobuf
RUN cd /tmp && git clone --recursive https://github.com/protocolbuffers/protobuf.git
RUN cd /tmp/protobuf && ./autogen.sh && ./configure && make && make check && make install && ldconfig
RUN rm -rf /tmp/protobuf

# install grpc
RUN cd /tmp &&  git clone --recurse-submodules -b v1.30.0 https://github.com/grpc/grpc
RUN cd /tmp/grpc && mkdir -p cmake/build
RUN cd /tmp/grpc/cmake/build && cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local ../..
RUN cd /tmp/grpc/cmake/build && make && make install && ldconfig
RUN rm -rf /tmp/grpc

# copy code
COPY . /app
RUN cd /app && git submodule update --init --recursive

# prepare gtest
RUN cd /app/lib && rm -rf googletest
RUN cd /app/lib && git clone https://github.com/google/googletest


## FULL FUNCTIONALITY

# installation
RUN rm -rf /app/build
RUN cd /app && mkdir build && cd build && cmake -G "Unix Makefiles" \
    -DBUILD_TESTS=ON \
    -DBUILD_GTEST=ON \
    -DBUILD_FOR_COVERAGE=OFF \
    -DCREATE_DOXYGEN_TARGET=ON \
    -DBUILD_MAP_SERVER=ON \
    -DCMAKE_BUILD_TYPE=Debug ..

# documentation, compilation, tests
RUN cd /app/build && make doxygen
RUN cd /app/build && make
RUN cd /app/build && make test


CMD bash