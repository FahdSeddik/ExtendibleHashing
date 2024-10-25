# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
TARGET = build/run

# Check if g++ is available
ifeq ($(shell which $(CXX)),)
    $(error Error: g++ compiler not found or not set up correctly. Please install g++ and ensure it's in your PATH.)
endif

all: $(TARGET)

# Create build directory if it doesn't exist
$(TARGET): $(OBJ)
	@mkdir -p build
	$(CXX) -o $@ $^

# Compile object files into the build directory
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the build directory
clean:
	rm -rf build
