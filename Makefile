# Variables
CXX = g++                # The C++ compiler
CXXFLAGS = -Wall -g      # Compiler flags (-Wall enables warnings, -g includes debug info)
TARGET = isa-top         # Name of the final executable
SRC = isa-top.cpp        # Source file

# Target to compile the source file into an executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Target to run the program
run: $(TARGET)
	./$(TARGET)

# Target to run the program with test arguments
run-test: $(TARGET)
	./$(TARGET) -i eth0 -s b

# Target to clean up the generated files (e.g., the executable)
clean:
	rm -f $(TARGET)
