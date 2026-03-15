CXX = g++
CXXFLAGS = -std=c++17 -O3 -I./include
LDFLAGS = -L./lib

# Detect OS
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    # Linux specific flags
    SYS_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME_S), Darwin)
    # macOS specific flags
    SYS_LIBS = -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif

# Ensure the bin directory exists
$(shell mkdir -p bin)

all: client server

client: src/client.cpp src/networking.cpp
	$(CXX) $(CXXFLAGS) src/client.cpp src/networking.cpp $(LDFLAGS) $(SYS_LIBS) -o bin/client

server: src/server.cpp src/networking.cpp
	$(CXX) $(CXXFLAGS) src/server.cpp src/networking.cpp -o bin/server

clean:
	rm -f bin/client bin/server