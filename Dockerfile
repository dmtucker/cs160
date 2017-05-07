FROM debian:jessie
MAINTAINER david@tucker.name
RUN apt-get update && apt-get install -y \
    g++=4:4.9.2-2 \
    libglew-dev=1.10.0-3 \
    libglfw3-dev=3.0.4-1 \
    libglm-dev=0.9.5.4-1 \
    make
WORKDIR /cs160
COPY . .
RUN make all
ENTRYPOINT ["/cs160/bin/main"]
