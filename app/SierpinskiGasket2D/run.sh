#!/bin/sh

PROJECT=main

make all
if [ $? -eq 0 ]
  then
    chmod +x bin/$PROJECT
    bin/$PROJECT $@
fi
