# OGbox

OpenGL Sandbox

``` sh
docker run -it \
  -e DISPLAY="$DISPLAY" -u "$(id -u)" -v /tmp/.X11-unix:/tmp/.X11-unix \
  dmtucker/ogbox:latest terrains/lab3.ppm textures/lab3.ppm
```

![Screenshot](https://github.com/dmtucker/ogbox/raw/master/screenshot.png)
