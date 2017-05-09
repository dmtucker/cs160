FROM debian:stretch
MAINTAINER david@tucker.name
RUN apt-get update && apt-get install -y \
    g++ \
    libglew-dev \
    libglfw3-dev \
    libglm-dev \
    make
WORKDIR /ogbox
COPY . .
RUN make all
ENTRYPOINT ["/ogbox/bin/ogbox"]
