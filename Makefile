# ============================================================================
# BUILD CONFIGURATION
# ============================================================================

ifeq ($(OS),Windows_NT)
    CC = gcc
    TARGET = optimeal.exe
    RM = del /Q
    CLEAR = cls
else
    CC = gcc
    TARGET = optimeal
    RM = rm -f
    CLEAR = clear
endif

CFLAGS = -Wall -Wextra -std=c99

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
	$(RM) $(OBJECTS) $(TARGET)

run: clean all
	$(CLEAR)
ifeq ($(OS),Windows_NT)
	.\$(TARGET)
else
	./$(TARGET)
endif

# ============================================================================
# DEPENDENCIES
# ============================================================================

# Install dependencies
install-deps:
ifeq ($(OS),Windows_NT)
	@echo "For Windows, please install MinGW-w64 or MSYS2"
	@echo "Download from: https://www.mingw-w64.org/downloads/"
	@echo "Or use Chocolatey: choco install mingw"
else
	@echo "For macOS/Linux, installing gcc..."
	brew install gcc
endif