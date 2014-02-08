#!/bin/sh

PROJECT=main

make again
if [ $? -eq 0 ]
  then
    chmod +x bin/$PROJECT
    bin/$PROJECT $@
fi
