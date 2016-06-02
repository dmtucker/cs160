FROM debian:jessie
MAINTAINER david.michael.tucker@gmail.com
RUN apt-get update && apt-get install -y \
    g++=4:4.9.2-2 \
    libglew-dev=1.10.0-3 \
    libglfw3-dev=3.0.4-1 \
    libglm-dev=0.9.5.1-1 \
    make
WORKDIR /src
COPY . .
RUN make all
ENTRYPOINT ["bash"]
