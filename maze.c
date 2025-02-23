#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 40
#define MAP_HEIGHT 24

// Reserve top 1 and bottom 2 lines for HUD
#define HUD_TOP 1
#define HUD_BOTTOM 2
#define PLAYABLE_HEIGHT (MAP_HEIGHT - HUD_TOP - HUD_BOTTOM)

// Define our cell grid dimensions based on the playable area.
// Each cell will produce a 3x3 open area, with walls in between.
// The cell grid dimensions are calculated from the playable width and height.
#define CELLS_X ((MAP_WIDTH - 1) / 4)         // For 40: (39 / 4) = 9 cells horizontally
#define CELLS_Y ((PLAYABLE_HEIGHT - 1) / 4)     // For 21: (20 / 4) = 5 cells vertically

// The maze dimensions once expanded from the cell grid.
#define MAZE_WIDTH  (CELLS_X * 4 + 1)           // e.g. 9*4+1 = 37
#define MAZE_HEIGHT (CELLS_Y * 4 + 1)           // e.g. 5*4+1 = 21

// Dimensions for the passage inside each cell.
#define CELL_WIDTH  3
#define CELL_HEIGHT 3

// The full map with HUD and maze; each line gets a null terminator.
char map[MAP_HEIGHT][MAP_WIDTH + 1];

// Data structure for a cell in our grid
typedef struct {
    int x, y;
} Cell;

// Simple stack for DFS (max size: all cells)
Cell stack[CELLS_X * CELLS_Y];
int stackSize = 0;

// Visited status for each cell in our grid.
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

// Utility: randomize an array of integers (used for random directions)
void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Maze-generation algorithm using a DFS recursive backtracker.
// We generate the maze in a temporary array 'maze' of size MAZE_HEIGHT x MAZE_WIDTH.
void carveMaze() {
    // Create temporary maze filled with walls.
    char maze[MAZE_HEIGHT][MAZE_WIDTH + 1];
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            maze[i][j] = '#';
        }
        maze[i][MAZE_WIDTH] = '\0';
    }
    
    // Start at cell (0, 0)
    int startX = 0, startY = 0;
    visited[startY][startX] = 1;
    int sx = startX * 4 + 1;
    int sy = startY * 4 + 1;
    // Carve out the 3x3 passage for the starting cell.
    for (int i = 0; i < CELL_HEIGHT; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) {
            maze[sy + i][sx + j] = '.';
        }
    }
    push((Cell){startX, startY});
    
    // DFS maze carving.
    while (!isEmpty()) {
        Cell current = stack[stackSize - 1]; // Peek at top of stack.
        int directions[4] = {0, 1, 2, 3}; // 0: up, 1: right, 2: down, 3: left.
        shuffle(directions, 4);
        int carved = 0;
        for (int k = 0; k < 4; k++) {
            int dir = directions[k];
            int nx = current.x, ny = current.y;
            if (dir == 0) ny--;       // Up.
            else if (dir == 1) nx++;  // Right.
            else if (dir == 2) ny++;  // Down.
            else if (dir == 3) nx--;  // Left.
            
            // Check boundaries for our cell grid.
            if (nx < 0 || nx >= CELLS_X || ny < 0 || ny >= CELLS_Y)
                continue;
            if (visited[ny][nx])
                continue;
            
            // Coordinates for current cell block in maze.
            int cx = current.x * 4 + 1;
            int cy = current.y * 4 + 1;
            // Coordinates for neighbor cell block.
            int nx_maze = nx * 4 + 1;
            int ny_maze = ny * 4 + 1;
            
            // Carve corridor between cells by clearing the wall between them.
            if (dir == 0) { // Up.
                int wallY = cy - 1;
                for (int j = 0; j < CELL_WIDTH; j++) {
                    maze[wallY][cx + j] = '.';
                }
            } else if (dir == 1) { // Right.
                int wallX = cx + CELL_WIDTH;
                for (int i = 0; i < CELL_HEIGHT; i++) {
                    maze[cy + i][wallX] = '.';
                }
            } else if (dir == 2) { // Down.
                int wallY = cy + CELL_HEIGHT;
                for (int j = 0; j < CELL_WIDTH; j++) {
                    maze[wallY][cx + j] = '.';
                }
            } else if (dir == 3) { // Left.
                int wallX = cx - 1;
                for (int i = 0; i < CELL_HEIGHT; i++) {
                    maze[cy + i][wallX] = '.';
                }
            }
            // Carve the neighbor cell's 3x3 passage.
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
    
    // Now copy the generated maze into our final map.
    // First fill the entire map with walls.
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[i][j] = '#';
        }
        map[i][MAP_WIDTH] = '\0';
    }
    
    // Center the maze within the playable area.
    // The playable area starts at row HUD_TOP and is PLAYABLE_HEIGHT lines tall.
    int playable_offsetY = HUD_TOP;
    int offsetX = (MAP_WIDTH - MAZE_WIDTH) / 2;
    int offsetY = playable_offsetY + (PLAYABLE_HEIGHT - MAZE_HEIGHT) / 2;
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            map[i + offsetY][j + offsetX] = maze[i][j];
        }
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    carveMaze();
    
    // Example: Print a top HUD line.
    printf("HUD: Score 0\n");
    
    // Print the playable area (from HUD_TOP to MAP_HEIGHT - HUD_BOTTOM).
    for (int i = HUD_TOP; i < MAP_HEIGHT - HUD_BOTTOM; i++) {
        printf("%s\n", map[i]);
    }
    
    // Example: Print bottom HUD lines.
    printf("Status: OK\n");
    printf("Commands: WASD to move\n");
    
    return 0;
}
