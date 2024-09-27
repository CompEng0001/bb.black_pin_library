# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror

# Target executable name
TARGET = adc2pwm

# Source files
SRC = adc2pwm.c

# Library to link against
LIBS = -lioctrl

# Default target: build the executable
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
        $(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(TARGET)

# Clean up build artifacts
clean:
        rm -f $(TARGET)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean