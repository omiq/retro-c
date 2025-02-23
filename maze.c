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
#define CELLS_X ((MAP_WIDTH - 1) / 4)         // For 40: (39 / 4) = 9 cells horizontally
#define CELLS_Y ((PLAYABLE_HEIGHT - 1) / 4)     // For 21: (20 / 4) = 5 cells vertically

// Maze dimensions once expanded from the cell grid.
#define MAZE_WIDTH  (CELLS_X * 4 + 1)           // e.g. 9*4+1 = 37
#define MAZE_HEIGHT (CELLS_Y * 4 + 1)           // e.g. 5*4+1 = 21

// Passage size within each cell.
#define CELL_WIDTH  3
#define CELL_HEIGHT 3

// Our final map array (each line ends with a null terminator).
char map[MAP_HEIGHT][MAP_WIDTH + 1];

typedef struct {
    int x, y;
} Cell;

// Stack for DFS maze generation.
Cell stack[CELLS_X * CELLS_Y];
int stackSize = 0;

// Visited flags for the cell grid.
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

// Utility function to randomize an array (used to shuffle directions).
void shuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Maze-generation algorithm using a DFS recursive backtracker.
void carveMaze() {
    // Create a temporary maze array (size MAZE_HEIGHT x MAZE_WIDTH) and fill with walls.
    char maze[MAZE_HEIGHT][MAZE_WIDTH + 1];
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            maze[i][j] = '#';
        }
        maze[i][MAZE_WIDTH] = '\0';
    }
    
    // Start at cell (0, 0) in the cell grid.
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
    
    // DFS to carve out the maze.
    while (!isEmpty()) {
        Cell current = stack[stackSize - 1]; // Peek at the top of the stack.
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
            
            // Check boundaries of the cell grid.
            if (nx < 0 || nx >= CELLS_X || ny < 0 || ny >= CELLS_Y)
                continue;
            if (visited[ny][nx])
                continue;
            
            // Coordinates in the maze for current cell.
            int cx = current.x * 4 + 1;
            int cy = current.y * 4 + 1;
            // Coordinates for neighbor cell.
            int nx_maze = nx * 4 + 1;
            int ny_maze = ny * 4 + 1;
            
            // Carve the wall between cells.
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
            // Carve the 3x3 block for the neighbor cell.
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
    
    // Copy the generated maze into our full map.
    // First fill the full map with walls.
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[i][j] = '#';
        }
        map[i][MAP_WIDTH] = '\0';
    }
    // Center the maze within the playable area.
    int playable_offsetY = HUD_TOP;
    int offsetX = (MAP_WIDTH - MAZE_WIDTH) / 2;
    int offsetY = playable_offsetY + (PLAYABLE_HEIGHT - MAZE_HEIGHT) / 2;
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            map[i + offsetY][j + offsetX] = maze[i][j];
        }
    }
}

// Place the player character '@' in a random valid position within the playable area.
// The candidate cell must be a '.' and have '.' immediately above, to the left, and to the right.
void placePlayer() {
    int row, col;
    // Restrict the random range so that we can safely check neighbors.
    // Rows from HUD_TOP+1 to MAP_HEIGHT - HUD_BOTTOM - 1,
    // Columns from 1 to MAP_WIDTH - 2.
    do {
        row = (rand() % (PLAYABLE_HEIGHT - 2)) + HUD_TOP + 1;
        col = (rand() % (MAP_WIDTH - 2)) + 1;
    } while (map[row][col] != '.' ||
             map[row-1][col] != '.' ||
             map[row+1][col] != '.' ||
             map[row][col-1] != '.' ||
             map[row][col+1] != '.');
    map[row][col] = '@';
}

int main(void) {
    srand((unsigned)time(NULL));
    carveMaze();
    placePlayer();
    
    // Print top HUD line.
    printf("HUD: Score 0\n");
    
    // Print the playable area.
    for (int i = HUD_TOP; i < MAP_HEIGHT - HUD_BOTTOM; i++) {
        printf("%s\n", map[i]);
    }
    
    // Print bottom HUD lines.
    printf("Status: OK\n");
    printf("Commands: WASD to move\n");
    
    return 0;
}
