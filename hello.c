#include <stdio.h>
#include <conio.h>
#include <cbm.h>
#include <peekpoke.h>
#include <stdlib.h>
#include <stdbool.h>

// Global key variable
bool in_play=false;
bool obstruction=false;
unsigned int key,i,c;
unsigned char x=19;
unsigned char y=8;
unsigned char old_x, old_y;
unsigned char level=0;
unsigned char buffer [sizeof(int)*8+1];

// Player 
unsigned char keys=0;
int health=100;
unsigned int score=0;



unsigned char game_map[] = {
     32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 11, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 94, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,  9, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 81, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 38, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 88, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102,102,102,102,102, 45,102,102,102,102,102,102,102,102,102, 32, 32, 32, 32, 32, 83, 32,102, 32, 32, 32, 32, 32, 32,
 32,102,102,102,102,102,102,102,102,102,102,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 36, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 11, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 83, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 11, 32, 32, 32, 32, 32,  9, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 88, 32, 32, 32, 32,  9, 32, 32, 32, 32, 38, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 94, 32, 32, 32, 32,102, 32, 32, 94, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 36, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102,102,102,102,102,102,102,102,102,102,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32

};

unsigned int map(char level, char x, char y) {
    
    unsigned char c;

    c = game_map[40*y+x];
    if(c>64) c+=64;
    // Right now just one map
    return c;
    
    // For testing you could use the following to output the charset
    // return 40*y+x;
}

void set_map(char level, char x, char y, int tile) {
    
    // Set the part of the array to the given tile
    game_map[40*y+x]=tile;
    
}

void draw_screen() {

    int row,col;
    for(row=0; row<25; row++)
    {
        for(col=0; col < 40; col++){
            cputcxy(col,row,map(level,col,row));
        }
    };

}


void petscii() {

    key=cgetc();
    i=0;
    for(x=0;x<40;x+=5) {
    for(y=0;y<20;y++) {
        gotoxy(x,y);
    	printf("%d",i);
        cputcxy(x+3,y,i+64);
        i++;
    }}
}

void draw_move(bool replace) {

    // Delete the player character
    if(!replace) {
        set_map(level, old_x, old_y, 32);
    }

    // Draw new location
    cputcxy(old_x,old_y,map(level,old_x,old_y));
    cputcxy(x,y,64); 
    
}

int main() {
    /* Clear Screen */
    clrscr();

    // Uppercase/Graphical characterset = 12
    POKE(59468,12);

// Character 102 at screen 0,0 
//    POKE(32768,102);

    draw_screen();
    // wait for keypress? key=cgetc();

    /* Hide cursor */
    cursor(0);
    cputcxy(x,y,'@');

    // Game on!
    in_play = true;

    /* Loop until Q is pressed */
    while (in_play)
    {

    gotoxy(0,24);
    printf("map %02d keys %02d health %03d score %03d", level, keys, health, score);

        // Backup the location
        old_x = x;
        old_y = y;

        // keys;
        switch (key=cgetc()) 
        { 
            case 'w':
                if(y>0) y--; 
                break; 
            case 'a': 
                if(x>0) x--; 
                break; 
            case 's': 
                if(y<24) y++; 
                break; 
            case 'd': 
                if(x<39) x++; 
                break; 
            case 'Q':
                in_play = false;
                break;
            default: 
                break; 
        }

        // Anything in our path?
        obstruction=false;
        c=map(level,x,y);
        
        // Collision
        switch (c)
        {
            case 166:
                // Wall               
                obstruction=true;
                break;
            
            case 11: // Key +1
                keys+=1;
                break;

            case 9:
            case 45:
                if(keys>0)
                {
                    keys-=1;
                    score+=10;
                    obstruction=false;

                }else{

                    // Not enough keys to unlock!
                    set_map(level, x, y, 28); // turn into partially open
                    health-=10; // lose 10 health
                    obstruction=true;
                }
                break;

            case 28: // Partially open door
                // Not enough keys to unlock!
                set_map(level, x, y, 32); // turn into fully open
                health-=10; // lose 10 health
                obstruction=true;
                break;

            case 36: // Cash money
                score+=50;
                break;
            
            case 38: // Gobbo
                health-=25;
                break;

            case 147: // Health
                health+=25;
                break;

            case 158: // Rats
                health-=5;
                break;

            case 152: // Idol
                score+=100;
                break;
            
            default:
//                itoa(c,buffer,10);
                gotoxy(0,0);
                printf("%03d",c);
                break;
        }

        // If obstructed then bounce
        if(obstruction) {
            x=old_x;
            y=old_y;
        } else {
            draw_move(false);
        }

        if(health<1) {
            clrscr();
            printf("ah, such a shame, you were doing so well!\n\nscore:%03d\n\n",score);
            key=cgetc();
            in_play=false;
        }
     
    }
    clrscr();
    printf("goodbye!\n\n");
    cursor(1);
    return(0);
}
