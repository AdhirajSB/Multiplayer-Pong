CXX = g++
CXXFLAGS = -std=c++17 -O3 -I./include

# Ensure the bin directory exists before compiling
$(shell mkdir -p bin)

# Build both by default if you just type 'make'
all: client server

# The Client links the macOS static library and your framework dependencies
client: src/client.cpp src/networking.cpp
	$(CXX) $(CXXFLAGS) src/client.cpp src/networking.cpp -L./lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -o bin/client

# The Server only compiles the standard C++ code (No Raylib!)
server: src/server.cpp src/networking.cpp
	$(CXX) $(CXXFLAGS) src/server.cpp src/networking.cpp -o bin/server

clean:
	rm -f bin/client bin/server