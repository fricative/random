CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Target executable name
TARGET = tango

# Source files
SRCS = main.cpp tango.cpp
HEADERS = tango.hpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean