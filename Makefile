# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -Llib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL

# Default target when you just type 'make'
all: client server

# Build the client
client: src/client.cpp src/networking.cpp
	$(CXX) $(CXXFLAGS) src/client.cpp src/networking.cpp -o bin/client $(LDFLAGS)

# Build the server
server: src/server.cpp src/networking.cpp
	$(CXX) $(CXXFLAGS) src/server.cpp src/networking.cpp -o bin/server $(LDFLAGS)

# Clean up generated files
clean:
	rm -f bin/client bin/server