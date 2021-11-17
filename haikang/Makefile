CC       = gcc
CXX      = g++
CFLAGS   = -g
CXXFLAGS = $(CFLAGS)

LIBPATH = ./
LIBS = -Wl,-rpath=./:./lib:./lib/HCNetSDKCom -lhcnetsdk
SRC = ./DeviceServiceIf.cpp ./DeviceHandler.cpp
TARGET = ./checkhealth

all: 
	$(CXX) $(OPTI) $(CXXFLAGS) $(SRC) -L$(LIBPATH) $(LIBS) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
