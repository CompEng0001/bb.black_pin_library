# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -fPIC -I./include  # Include headers
AR = ar
ARFLAGS = rcs
PREFIX = /usr
INCLUDEDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib

# Directories
SRCDIR = ./src
OBJDIR = ./obj

# Library names
TARGET_STATIC = libioctrl.a
TARGET_SHARED = libioctrl.so

# Source files for each component
SRC = $(SRCDIR)/gpio.c $(SRCDIR)/pwm.c $(SRCDIR)/adc.c
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

# Default target: Build both libraries
all: $(TARGET_STATIC) $(TARGET_SHARED)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the static library
$(TARGET_STATIC): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

# Create the shared library
$(TARGET_SHARED): $(OBJ)
	$(CC) -shared -o $@ $(OBJ)

# Clean up build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET_STATIC) $(TARGET_SHARED)

# Install libraries and headers
install: $(TARGET_STATIC) $(TARGET_SHARED)
	sudo cp include/gpio.h include/pwm.h include/adc.h include/libioctrl.h $(INCLUDEDIR)/
	sudo cp $(TARGET_STATIC) $(TARGET_SHARED) $(LIBDIR)/
	sudo ldconfig

# Uninstall libraries and headers
uninstall:
	sudo rm -f $(INCLUDEDIR)/gpio.h $(INCLUDEDIR)/pwm.h $(INCLUDEDIR)/adc.h $(INCLUDEDIR)/libioctrl.h
	sudo rm -f $(LIBDIR)/$(TARGET_STATIC) $(LIBDIR)/$(TARGET_SHARED) $(OBJDIR)
	sudo ldconfig

# Phony targets
.PHONY: all clean install uninstall
