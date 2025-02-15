#include <stdio.h>
#include <cpm.h>

/* BDOS function codes */
#define CONSOLE_STATUS 0x0B
#define CONSOLE_INPUT 0x06

void main() {
    int ch;

    while (1) {

        /* Check console status */
        if (bdos(CONSOLE_STATUS, 0) != 0) {

            /* Character available - read it */
            ch = bdos(CONSOLE_INPUT, 0xFF);
            printf("Key pressed: %c\n", ch);
            if (ch == 'q') {
                break;
            }
        } else {

            /* No key available - continue with other tasks */
            printf("No key pressed, doing other work...\n");
        }
    }

    return;
}
