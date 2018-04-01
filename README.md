# OGbox

An OpenGL Sandbox

[![Build Status](https://img.shields.io/travis/dmtucker/ogbox.svg)](https://travis-ci.org/dmtucker/ogbox)

![Screenshot](https://github.com/dmtucker/ogbox/raw/master/screenshot.png)

## Quickstart

``` sh
docker build -t ogbox . && docker run -it \
  -u $(id -u) \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  ogbox terrains/lab3.ppm textures/lab3.ppm
```
