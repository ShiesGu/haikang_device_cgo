#!/bin/bash

g++ -o DeviceServiceIf.o -c DeviceServiceIf.cpp
g++ -o DeviceHandler.o -c DeviceHandler.cpp
ar r libdevice_handler.so DeviceHandler.o DeviceServiceIf.o