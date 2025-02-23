#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 40
#define MAP_HEIGHT 24

// We will work on a grid of "cells" where each cell will be expanded to a 3x3 open block.
// Between cells we leave a wall (1 cell thick).
// With a border of walls, the maze dimensions become:
//   MAZE_WIDTH  = CELLS_X * 4 + 1
//   MAZE_HEIGHT = CELLS_Y * 4 + 1
// For our 40x24 map, if we use integer division we get:
#define CELLS_X ((MAP_WIDTH - 1) / 4)  // For 40: (39 / 4) = 9 cells horizontally
#define CELLS_Y ((MAP_HEIGHT - 1) / 4) // For 24: (23 / 4) = 5 cells vertically

#define MAZE_WIDTH  (CELLS_X * 4 + 1)   // 9*4+1 = 37
#define MAZE_HEIGHT (CELLS_Y * 4 + 1)   // 5*4+1 = 21

// The passage inside each cell is 3x3.
#define CELL_WIDTH  3
#define CELL_HEIGHT 3

// Our final map will be stored here (and we’ll add a null terminator for each line)
char map[MAP_HEIGHT][MAP_WIDTH + 1];

// Data structure for the maze cell coordinates (in our cell grid)
typedef struct {
    int x, y;  // x in [0, CELLS_X-1], y in [0, CELLS_Y-1]
} Cell;

// For DFS we use a simple stack (max size is all cells)
Cell stack[CELLS_X * CELLS_Y];
int stackSize = 0;

// Keep track of which cells have been visited
int visited[CELLS_Y][CELLS_X] = {0};

void push(Cell c) {
    stack[stackSize++] = c;
}

Cell pop() {
    return stack[--stackSize];
}

int isEmpty() {
    return stackSize == 0;
}

// Utility to shuffle an array of integers in place (for randomizing directions)
void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// The maze-generation algorithm.
// It works on a smaller grid (of size CELLS_Y x CELLS_X) and “carves” out a maze.
// We build the maze in a temporary array 'maze' (of size MAZE_WIDTH x MAZE_HEIGHT)
// where every cell is initially a wall ('#').
// When we "carve" a cell we set a 3x3 block to the passage character ('.').
// When carving a connection between two adjacent cells we also clear the wall between them.
void carveMaze() {
    // Temporary maze array
    char maze[MAZE_HEIGHT][MAZE_WIDTH + 1];
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            maze[i][j] = '#';
        }
        maze[i][MAZE_WIDTH] = '\0';
    }

    // Start at cell (0, 0) (you could randomize this if desired)
    int startX = 0, startY = 0;
    visited[startY][startX] = 1;
    // Compute top-left coordinates of the cell block in the maze:
    int sx = startX * 4 + 1;
    int sy = startY * 4 + 1;
    // Carve out the 3x3 block for the starting cell
    for (int i = 0; i < CELL_HEIGHT; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) {
            maze[sy + i][sx + j] = '.';
        }
    }
    push((Cell){startX, startY});

    // DFS to carve the maze
    while (!isEmpty()) {
        Cell current = stack[stackSize - 1]; // peek at top of stack
        // Directions: 0 = up, 1 = right, 2 = down, 3 = left
        int directions[4] = {0, 1, 2, 3};
        shuffle(directions, 4);
        int carved = 0;
        for (int k = 0; k < 4; k++) {
            int dir = directions[k];
            int nx = current.x, ny = current.y;
            if (dir == 0) ny--;       // up
            else if (dir == 1) nx++;  // right
            else if (dir == 2) ny++;  // down
            else if (dir == 3) nx--;  // left

            // Check boundaries of the cell grid
            if (nx < 0 || nx >= CELLS_X || ny < 0 || ny >= CELLS_Y)
                continue;
            if (visited[ny][nx])
                continue;

            // Coordinates in the maze for the current cell block:
            int cx = current.x * 4 + 1;
            int cy = current.y * 4 + 1;
            // And for the neighbor cell block:
            int nx_maze = nx * 4 + 1;
            int ny_maze = ny * 4 + 1;

            // Carve a corridor between the current cell and neighbor cell.
            // Since our passages are 3 cells wide, we clear the wall between them.
            if (dir == 0) {
                // Up: clear the row just above the current block.
                int wallY = cy - 1;
                for (int j = 0; j < CELL_WIDTH; j++) {
                    maze[wallY][cx + j] = '.';
                }
            } else if (dir == 1) {
                // Right: clear the column just to the right.
                int wallX = cx + CELL_WIDTH;
                for (int i = 0; i < CELL_HEIGHT; i++) {
                    maze[cy + i][wallX] = '.';
                }
            } else if (dir == 2) {
                // Down: clear the row just below.
                int wallY = cy + CELL_HEIGHT;
                for (int j = 0; j < CELL_WIDTH; j++) {
                    maze[wallY][cx + j] = '.';
                }
            } else if (dir == 3) {
                // Left: clear the column just to the left.
                int wallX = cx - 1;
                for (int i = 0; i < CELL_HEIGHT; i++) {
                    maze[cy + i][wallX] = '.';
                }
            }
            // Carve the neighbor's 3x3 cell block.
            for (int i = 0; i < CELL_HEIGHT; i++) {
                for (int j = 0; j < CELL_WIDTH; j++) {
                    maze[ny_maze + i][nx_maze + j] = '.';
                }
            }
            visited[ny][nx] = 1;
            push((Cell){nx, ny});
            carved = 1;
            break;
        }
        if (!carved) {
            pop();
        }
    }

    // Now, copy the generated maze into our final 40x24 map.
    // First fill the whole map with walls.
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[i][j] = '#';
        }
        map[i][MAP_WIDTH] = '\0';
    }
    // Center the maze within the map.
    int offsetX = (MAP_WIDTH - MAZE_WIDTH) / 2;
    int offsetY = (MAP_HEIGHT - MAZE_HEIGHT) / 2;
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            map[i + offsetY][j + offsetX] = maze[i][j];
        }
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    carveMaze();
    // Print the final map
    for (int i = 0; i < MAP_HEIGHT; i++) {
        printf("%s\n", map[i]);
    }
    return 0;
}
