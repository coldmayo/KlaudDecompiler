# Define the compiler and the flags
CC = gcc
CFLAGS = -g -Wall -Wextra -O2

# Define the target executable
TARGET = dis

# Define the source files and the corresponding object files
SRCS = main.c decomp.c
OBJS = $(SRCS:.c=.o)

# Define the headers (for dependency generation)
DEPS = decomp.h

# Default target to build the executable
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile each source file into an object file
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the generated files
clean:
	rm -f $(OBJS) $(TARGET)

run:
	@./dis
# Phony targets
.PHONY: all clean
