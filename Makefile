CXX = g++
CXXFLAGS = -std=c++17 -O3 -I./include

# 1. Detect which Operating System we are on
UNAME_S := $(shell uname -s)

# 2. Set the flags based on the OS
ifeq ($(UNAME_S), Linux)
    # LINUX FLAGS: Uses system-installed raylib
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else
    # MAC FLAGS: Uses your local ./lib folder and Apple Frameworks
    LIBS = -L./lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif

# Ensure the bin directory exists
$(shell mkdir -p bin)

all: client server

client: src/client.cpp src/networking.cpp
	$(CXX) $(CXXFLAGS) src/client.cpp src/networking.cpp $(LIBS) -o bin/client

server: src/server.cpp src/networking.cpp
	$(CXX) $(CXXFLAGS) src/server.cpp src/networking.cpp -o bin/server

clean:
	rm -f bin/client bin/server