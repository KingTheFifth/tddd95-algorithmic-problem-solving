TARGET = stammering 
CXX = g++
CXXFLAGS = -O2 -std=gnu++17
LDFLAGS = -lrt -lpthread

SRCS = $(wildcard *.cpp)

$(TARGET): $(SRCS)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

