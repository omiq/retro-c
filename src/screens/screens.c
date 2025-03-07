#include "screens.h"
#include "../display/display.h"
#include "../input/input.h"

#if defined (__CC65__)
#include <conio.h>
#else
#include "../include/notconio.h"
#endif

// This is the default title screen
int title_screen(void) {

    // if c64 then set the background color to black and the text to green
    #ifdef __C64__
        bgcolor(0);
        bordercolor(0);
        textcolor(5);
    #endif

    clrscr();
    
    sprintf(output, "pet dungeon");
    cputsxy(11, 10, output);
    
    sprintf(output, "a game by retrogamecoders.com");
    cputsxy(2, 15, output);
    
    sprintf(output, "press a key");
    cputsxy(12, 20, output);

    counter = 0;
    while (!kbhit()) { counter++; }
    in_play = true;
    clrscr();
    return counter;
}

bool game_over(void) {
    clrscr();
    sprintf(output, " game over\n\n");
    cputsxy(15, 10, output);
    refresh();
    timer = dumb_wait(1000);
    sprintf(output, " ah, such a shame,");
    cputsxy(16, 12, output);
    sprintf(output, "you were doing so well!");
    cputsxy(16, 14, output);
    timer = dumb_wait(1000);
    refresh();
    sprintf(output, " score:%03d", score);
    cputsxy(20, 18, output);
    sprintf(output, " play again (y/n)?");
    cputsxy(20, 19, output);
    refresh();
    key = cgetc();
    in_play = false;
    if (key == 'n') {
        cursor(1);
        echo();
        refresh();
        return false;
    } else {
        return true;
    }
} 