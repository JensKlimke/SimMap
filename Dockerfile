FROM ubuntu:18.04
MAINTAINER Jens Klimke (jens.klimke@rwth-aachen.de)

# installation
RUN apt-get update
RUN apt-get -y install git g++ make cmake nano doxygen graphviz nodejs npm wget
RUN cmake --version && g++ --version

# update node
RUN npm cache clean -f && npm install -g n && n stable

# copy code
COPY . /app
RUN cd /app && git submodule update --init --recursive

# prepare inta
RUN cd /app && git clone https://github.com/google/googletest



## FULL FUNCTIONALITY

# installation
RUN rm -rf /app/build
RUN cd /app && mkdir build && cd build && cmake -G "Unix Makefiles" \
    -DBUILD_TESTS=ON \
    -DBUILD_GTEST=ON \
    -DBUILD_FOR_COVERAGE=OFF \
    -DCREATE_DOXYGEN_TARGET=ON \
    -DCMAKE_BUILD_TYPE=Debug ..

# documentation, compilation, tests
RUN cd /app/build && make doxygen
RUN cd /app/build && make
RUN cd /app/build && make test


CMD bash