# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Executable
TARGET = benchmark

# Source files
SRCS = benchmark.cpp ae.cpp tick_data.cpp haar_wavelet.cpp

# Header files
HEADERS = ae.hpp tick_data.hpp haar_wavelet.hpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Compile and link the project
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the project directory
clean:
	rm -f $(TARGET) $(OBJS)

