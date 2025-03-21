CXX = g++-13
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -g
TARGET = queens
SRCS = main.cpp queens.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = queens.hpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean