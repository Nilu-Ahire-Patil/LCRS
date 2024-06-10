# Compiler and flags --------------------------------------------
CC = g++
CFLAGS = -Wall -Iinclude -g -o3 -fPIC
# CFLAGS = -Wall -Iinclude -fPIC

# Directories ---------------------------------------------------
SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin
LIBDIR = lib
LOGDIR = log
ETCDIR = etc

# Shared library ------------------------------------------------
SHARED_LIB = $(LIBDIR)/liblcrs.so

# Source files and object files ---------------------------------
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# Target executable ---------------------------------------------
TARGET = $(BINDIR)/lcrs

# Default rule --------------------------------------------------
all: $(TARGET)
	@echo "Done."
	
# Building the shared library -----------------------------------
$(SHARED_LIB): $(OBJECTS) | $(LIBDIR)
	@echo "\nCreating shared library file..."
	@$(CC) -shared -o $(SHARED_LIB) $(OBJECTS)

# Linking the executable with the shared library ----------------
$(TARGET): $(BUILDDIR)/main.o $(SHARED_LIB) | $(BINDIR)
	@echo "Creating Executable..."
	@$(CC) $(CFLAGS) -o $(TARGET) $(BUILDDIR)/main.o \
		-L$(LIBDIR) -Wl,-rpath,'$$ORIGIN/../lib' \
		-llcrs -luuid

# Compiling the source files ------------------------------------
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@echo -n "."
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -n ".."

# Compiling the main file ---------------------------------------
$(BUILDDIR)/main.o: main.cpp | $(BUILDDIR) $(LOGDIR)
	@$(CC) $(CFLAGS) -c main.cpp -o $(BUILDDIR)/main.o

# Create directory if it doesn't exist --------------------------
# creating build directory
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

# creating bin directory
$(BINDIR):
	@mkdir -p $(BINDIR)

# creating lib directory
$(LIBDIR):
	@mkdir -p $(LIBDIR)

# creating log directory
$(LOGDIR):
	@mkdir -p $(LOGDIR)

# creating etc directory
$(ETCDIR):
	@mkdir -p $(ETCDIR)

# Clean up ------------------------------------------------------
.PHONY: clean
clean:
# remove build directory and all files
	@if [ -d "$(BUILDDIR)" ]; then \
		echo "Removing local files."; \
		rm -f $(TARGET) $(BUILDDIR)/main.o \
		$(SHARED_LIB) $(OBJECTS); rm -r $(BUILDDIR); \
	fi
# remove bin directory
	@if [ -d "$(BINDIR)" ]; then \
		echo "Removing bin."; \
		rm -r $(BINDIR); \
	fi
# remove lib directory
	@if [ -d "$(LIBDIR)" ]; then \
		echo "Removing lib."; \
	       	rm -r $(LIBDIR); \
	fi
# remove log directory
	@if [ -d "$(LOGDIR)" ]; then \
		echo "Removing log."; \
		rm -r $(LOGDIR); \
	fi
	@echo "Done."

# Install shared library & executable to system directories -----
install: $(TARGET)
# install library files
	@echo "Installing library files." 
	@sudo cp $(SHARED_LIB) /usr/local/lib 

# install executables
	@echo "Installing executable." 
	@sudo cp $(TARGET) /usr/local/bin 

	@echo "Done."

# Uninstall shared library & executable from system directories -
uninstall:	# clean 
# check for libbrary is present. remove if present
	@if [ -f "/usr/local/lib/liblcrs.so" ]; then \
		echo "Removing lib files..."; \
	       	rm "/usr/local/lib/liblcrs.so"; \
	fi
# check for executable is present. remove if present
	@if [ -f "/usr/local/bin/lcrs" ]; then \
		echo "Removing Executable..."; \
	       	rm "/usr/local/bin/lcrs"; \
	fi
	@echo "Done."
