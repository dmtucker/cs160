FROM debian:stretch
MAINTAINER david@tucker.name
RUN apt-get update && apt-get install -y \
    g++=4:6.3.0-4 \
    indent=2.2.11-5 \
    libglew-dev=2.0.0-3+b1 \
    libglfw3-dev=3.2.1-1 \
    libglm-dev=0.9.8.3-3 \
    make=4.1-9.1
WORKDIR /ogbox
COPY . .
RUN make all
RUN make pretty && for file in src/*.cpp src/*.hpp; do diff "$file" "src/${file##*/}~"; done
ENTRYPOINT ["/ogbox/bin/ogbox"]
