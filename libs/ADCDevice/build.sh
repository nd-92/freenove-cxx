#!/bin/sh

g++ -fPIC -c ADCDevice.C
g++ -shared -fPIC -o libADCDevice.so ADCDevice.o
sudo cp ADCDevice.H /usr/include/
sudo cp libADCDevice.so /usr/lib
sudo ldconfig

echo "build completed!"
