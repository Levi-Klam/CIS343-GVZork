
# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -Wextra -std=c++17 -mconsole

# Executable Name
TARGET = game.exe

# Source Files
SRCS = game.cpp main.cpp

# Object Files
OBJS = $(SRCS:.cpp=.o)

# Default Rule
all: $(TARGET)

# Link Objects into Executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile Source Files into Object Files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Compiled Files
clean:
	rm -f $(OBJS) $(TARGET)
