#include <stdio.h>

/* Declaration for CP/M BDOS call */
#define BDOS 0x0005

/* BDOS function codes */
#define CONSOLE_STATUS 0x0B
#define CONSOLE_INPUT 0x06

/* Inline assembly to call BDOS */
int cpm_bdos(int func, int param) {
    int result;
    __asm
    ld c, l       ; func in C register
    ld e, d       ; param in E register
    call BDOS     ; call CP/M BDOS
    ld l, a       ; result in L
    __endasm;
    result = 0; /* Placeholder, since inline assembly varies */
    return result;
}

int main() {
    int ch;

    while (1) {
        /* Check console status */
        if (cpm_bdos(CONSOLE_STATUS, 0) != 0) {
            /* Character available - read it */
            ch = cpm_bdos(CONSOLE_INPUT, 0xFF);
            printf("Key pressed: %c\n", ch);
            if (ch == 'q') {
                break;
            }
        } else {
            /* No key available - continue with other tasks */
            printf("No key pressed, doing other work...\n");
        }
    }

    return 0;
}
