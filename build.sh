#!/bin/bash
c++ triangulate-tool.cpp -o triangulate-tool $(pkg-config --cflags-only-I opencv4) -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
