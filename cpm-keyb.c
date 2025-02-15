/** non-blocking keyboard check in CP/M **/

#include <stdio.h>
#include <conio.h>

int main() {
    int c;

    while (1) {
        if (kbhit()) { // Check if a key is pressed
            c = getch(); // Non-blocking read of the character
            printf("Key pressed: %c\n", c);
            if (c == 'q') break; // Exit on 'q'
        } else {
            // No key pressed; continue with other tasks
            printf("No key pressed, doing other work...\n");
        }
    }

    return 0;
}
