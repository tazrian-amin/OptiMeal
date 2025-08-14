# ============================================================================
# BUILD CONFIGURATION
# ============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = optimeal

# ============================================================================
# SOURCE FILES
# ============================================================================

SOURCES = main.c \
          modules/user.c \
          modules/owner.c \
          modules/admin.c \
          modules/database.c \
          modules/utils.c \
          modules/auth.c \
          modules/business.c \
          modules/ui.c

OBJECTS = $(SOURCES:.c=.o)

# ============================================================================
# BUILD TARGETS
# ============================================================================

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================================
# UTILITY TARGETS
# ============================================================================

clean:
	rm -f $(OBJECTS) $(TARGET)

run: clean all
	clear
	./$(TARGET)

# ============================================================================
# DEPENDENCIES
# ============================================================================

# Install dependencies (for macOS)
install-deps:
	brew install gcc