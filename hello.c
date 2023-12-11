#include <stdio.h>
#include <conio.h>


// Global key variable
int key;
char x=19;
char y=10;

void fill_screen() {

    int row,col;
    for(row=0; row<25; row++)
    {
        for(col=0; col < 40; col++){
            cputsxy(col,row,".");
        }
    };

}

int main() {
    /* Clear Screen */
    clrscr();
    fill_screen();

    /* Hide cursor */
    cursor(0);
    cputsxy(x,y,"@");

    /* Loop until Q is pressed */
    while ((key  = cgetc()) != 'Q')
    {
        // Delete the character
        cputsxy(x,y,".");
        // keys;
        switch (key) 
        { 
            case 'w': 
                y--; 
                break; 
            case 'a': 
                x--; 
                break; 
            case 's': 
                y++; 
                break; 
            case 'd': 
                x++; 
                break; 
            default: 
                break; 
        }
        cputsxy(x,y,"@");
     
    }
    cursor(1);
    return(0);
}
