CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lncurses

# Source files
SRC = main.c \
      include/globals.c \
      include/maze.c \
      include/notconio.c \
      display/display.c \
      input/input.c \
      logic/game_logic.c \
      screens/screens.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable name
TARGET = dungeon_modular

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean 