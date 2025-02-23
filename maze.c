#include "maze.h"


int main(void) {
    int i;
    srand((unsigned)time(NULL));
    carveMaze();
    placePlayer();

    /* Print top HUD line */
    printf("HUD: Score 0\n");

    /* Print the playable area */
    for (i = HUD_TOP; i < MAP_HEIGHT - HUD_BOTTOM; i++) {
        printf("%s\n", map[i]);
    }

    /* Print bottom HUD lines */
    printf("Status: OK\n");
    printf("Commands: WASD to move\n");

    return 0;
}
