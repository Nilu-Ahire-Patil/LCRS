# Compiler and flags
CC = g++
CFLAGS = -Wall -Iinclude -o3 -fPIC

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin
LIBDIR = lib

# Shared library
SHARED_LIB = $(LIBDIR)/liblcrs.so

# Source files and object files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# Target executable
TARGET = $(BINDIR)/lcrs

# Default rule
all: $(TARGET)
	@echo "Done"
	
# Building the shared library
$(SHARED_LIB): $(OBJECTS) | $(LIBDIR)
	@echo "\nCreating shared library file..."
	@$(CC) -shared -o $(SHARED_LIB) $(OBJECTS)

# Linking the executable with the shared library
$(TARGET): main.o $(SHARED_LIB) | $(BINDIR)
	@echo "Creating Executable..."
	@$(CC) $(CFLAGS) -o $(TARGET) main.o -L$(LIBDIR) -Wl,-rpath,'$$ORIGIN/../lib' -llcrs

# Compiling the source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@echo -n "."
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -n ".."

# Compiling the main file
main.o: main.cpp
	@$(CC) $(CFLAGS) -c main.cpp -o main.o

# Create build directory if it doesn't exist
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

# Create bin directory if it doesn't exist
$(BINDIR):
	@mkdir -p $(BINDIR)

# Create lib directory if it doesn't exist
$(LIBDIR):
	@mkdir -p $(LIBDIR)

# Clean up
.PHONY: clean
clean:
	@echo "Cleaning local files..."
	@if [ -d "$(BUILDDIR)" ]; then rm -f $(TARGET) $(SHARED_LIB) $(OBJECTS); rmdir $(BUILDDIR); fi
	@echo "Removing local Directories..."
	@if [ -d "$(BINDIR)" ]; then rmdir $(BINDIR); fi
	@if [ -d "$(LIBDIR)" ]; then rmdir $(LIBDIR); fi
	@echo "Done."

# Install the shared library and executable to system directories
install: $(TARGET)
	@echo "Installing..."
	@sudo cp $(SHARED_LIB) /usr/local/lib
	@sudo cp $(TARGET) /usr/local/bin
	@echo "Done."

# Uninstall the shared library and executable to system directories
uninstall:	# clean 
	@echo "Removing lib files..."
	@if [ -f "/usr/local/lib/liblcrs.so" ]; then rm "/usr/local/lib/liblcrs.so"; fi
	@echo "Removing Executable..."
	@if [ -f "/usr/local/bin/lcrs" ]; then rm "/usr/local/bin/lcrs"; fi
	@echo "Done."
