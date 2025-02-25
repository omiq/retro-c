/*

This version will form the basis using modern ascii
conversions will then use header files and definitions

 cc dungeon.c -o ~/dungeon -lncurses

 install ncurses: sudo apt-get install libncurses-dev

*/

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <unistd.h>
#include <string.h>


// Global variables due to CC65 not liking local
bool run=true;
bool in_play=false;
bool obstruction=false;
unsigned char info_row = 23;
unsigned int this_row;
unsigned int this_col;
unsigned int timer,delay;
unsigned char key,i,c;
unsigned char player_x=19;
unsigned char player_y=8;
unsigned char old_x, old_y, direction_x, direction_y, fx, fy;
unsigned char room=1;
unsigned char buffer [sizeof(int)*8+1];
char output [256];


#if defined (__CC65__)
#include<cc65.h>
#include <conio.h>

#if defined __CBM__
    #include <cbm.h>
    #include <peekpoke.h>

    #define VIC_BASE_RAM			(0x8000)
    #define SCREEN_RAM				((char*)VIC_BASE_RAM+0x0400)
    #define CHARMAP_RAM				((char*)VIC_BASE_RAM+0x0800)
    #define SCREEN_WIDTH			40
    #define SCREEN_HEIGHT			25
    #define COLOR_OFFSET			(int)(COLOUR_RAM - SCREEN_RAM)
    #define CHARS_RAM			    0x80
    #define CHAR_SPACE				0x20
    #define RAM_BLOCK				(CHARS_RAM + CHAR_SPACE)
    #define CHARMAP_DEST			(RAM_BLOCK + 1)


unsigned int dumb_wait(unsigned int delay) {
    for(timer=0; timer<delay; timer++) {}
    return timer;
}



#endif
#else
    #include "notconio.h"
    #include <time.h>


    unsigned int dumb_wait(unsigned int delay) {
   

    // Storing start time
    clock_t start_time = clock();
    int counter = 0;
    // looping till required time is not achieved
    while (clock() < start_time + delay*250)
        gotoxy(0,0);
        counter++;
        //printf("%d",counter);
        ;
    return delay;
}

#endif



// Player 
unsigned char keys,idols,potion=0;
int health=100;
int magic=100;
unsigned int score=0;
bool sword = false;

// Enemy
unsigned int enemy_count=0;
struct enemy {
    unsigned char tile;
    unsigned char room;
    unsigned char x;
    unsigned char y;
    unsigned char old_x;
    unsigned char old_y;
    char health;
    unsigned char strength;
    unsigned char speed;
    unsigned char armour;
};
unsigned int this_enemy = 0;

struct enemy enemies[1000];



unsigned char title_screen_data[] = {};


#include "maze.h"

#if defined (__CC65__)

    void output_message() {

        printf(output);
        output[0] = '\0';
    }


#else
    void output_message() {
        printw("%s", output);
        output[0] = '\0';
    }


#endif






unsigned char game_map[1000];

unsigned char get_map(char x, char y) {
    
    unsigned char c;

    c = game_map[40*y+x];
    return c;
  

}


void load_room() {
    int pos=0;

    srand((unsigned)time(NULL));
    carveMaze();
    placePlayer();
    placeObject('G');

    // 1 piece of idol per room level
    for(i=0; i<room; i++) placeObject('I');
    
    placeObject('R');
    placeObject('*');
    placeObject('/');
    placeObject('$');
    placeObject('H');
    placeObject('K');


    clrscr();
    gotoxy(0,info_row);
    sprintf(output, "loading room %d",room);
    output_message();
   

    for (this_row = 0; this_row < 24; this_row++) {  
        for(this_col = 0; this_col < 40; this_col++) { 
        
        c=map[this_row][this_col];

        // Player x and y
        if(c=='@') {
            player_y=this_row;
            player_x=this_col;
        }

        // Goblin
        if(c=='G') {

            // Increment for next enemy (Enemy 0 is counted as no enemy)
            enemy_count+=1;

            // Create the enemy in the list
            enemies[enemy_count].tile = c;
            enemies[enemy_count].room = room;
            enemies[enemy_count].x = this_col;
            enemies[enemy_count].y = this_row;
            enemies[enemy_count].old_x = enemies[enemy_count].x;
            enemies[enemy_count].old_y = enemies[enemy_count].y;
            enemies[enemy_count].health = 30;
            enemies[enemy_count].strength = 5;
            enemies[enemy_count].speed = 1;
            enemies[enemy_count].armour = 10;

        }  

        // Rat
        else if (c=='R') {

            // Increment for next enemy (Enemy 0 is counted as no enemy)
            enemy_count+=1;

            // Create the enemy in the list
            enemies[enemy_count].tile = c;
            enemies[enemy_count].room = room;
            enemies[enemy_count].x = this_col;
            enemies[enemy_count].y = this_row;
            enemies[enemy_count].old_x = enemies[enemy_count].x;
            enemies[enemy_count].old_y = enemies[enemy_count].y;
            enemies[enemy_count].health = 15;
            enemies[enemy_count].strength = 5;
            enemies[enemy_count].speed = 2;
            enemies[enemy_count].armour = 0;

        }  
        if(c=='.') c=32;
        game_map[pos] = c;   
        pos++;  
        }
    }
    
    /*
    sprintf(output, "\n\nenemies %3d\n", enemy_count);
    for(i=1; i<enemy_count+1;i++)
    {
        sprintf(output, "%s %3d\n",enemies[i].tile,i);
    }

    key = cgetc();      
    */
}

void set_map(char x, char y, int tile) {
    
    // Set the part of the array to the given tile
    game_map[40*y+x]=tile;
    
}





// Returns the enemy for a given x,y coord
unsigned int which_enemy(unsigned int ex,unsigned int ey) {

    if(get_map(ex,ey)==32) return 0;

    // Enemies starts at 1, 0 = no enemy
    for(i=1;i<enemy_count+1;i++)
    {
        if(enemies[i].x == ex && enemies[i].y == ey && enemies[i].health >= 1) return i;
       
    }

    // No enemies
    return 0;

}


void attack(unsigned int weapon, unsigned int ax, unsigned int ay)
{
    int rnum = 0;
    this_enemy = 0;
    this_enemy = which_enemy(ax,ay);
    if(this_enemy == 0) {
        return;
    } 

    rnum = (rand() % (20 - 1 + 1)) + 1; 

    if(rnum > enemies[this_enemy].armour+enemies[this_enemy].speed) {

        // Damage!
        enemies[this_enemy].health-=weapon;
        if(enemies[this_enemy].health>0) 
        {
            gotoxy(0,info_row);
            
            sprintf(output, "hit!! enemy health: %3d    ", enemies[this_enemy].health);
            output_message();
            timer=dumb_wait(1000);
        }


        
    } else {
        gotoxy(0,info_row);

        sprintf(output, "miss! enemy health: %3d    ", enemies[this_enemy].health);
        output_message();
        if((player_x == ax && player_y == ay)||(player_x == ax && (player_y == ay + 1 || player_y == ay - 1)) || (player_y == ay && (player_x == ax + 1 || player_x == ax - 1))) 
        {
            health -= enemies[this_enemy].strength;
        }
        timer=dumb_wait(1000);
    }

    if(enemies[this_enemy].health < 1 ) {

        // Success!
        gotoxy(0,info_row);
        sprintf(output, "enemy defeated!                      ");
        output_message();
        // Draw tile in new location
        cputcxy(ax,ay,32); 
        set_map(ax,ay,32);
        enemies[this_enemy].tile = 32;

        // Up the score
        score+=10;
        timer=dumb_wait(1000);
    }

}

void enemy_attack(unsigned int this_enemy)
{
    int rnum = 0;
    rnum = (rand() % (20 - 1 + 1)) + 1; 

    if(rnum > 10) {

        // Damage!
        if(health < 1 || (health-enemies[this_enemy].strength) < 1) 
        {
            health = 0;

        } else {
            health-=enemies[this_enemy].strength;
        }    
        gotoxy(0,info_row);
        sprintf(output, "ouch! health: %3d        ", health);
        output_message();
        timer=dumb_wait(1000);

        
    } else {
            enemies[this_enemy].health -= 5;
            if(enemies[this_enemy].health<1) {
               enemies[this_enemy].health=0;
                // Draw tile in new location
                cputcxy(enemies[this_enemy].x,enemies[this_enemy].y,32); 
                set_map(enemies[this_enemy].x,enemies[this_enemy].y,32);
                enemies[this_enemy].tile = 32;
                gotoxy(0,info_row);
                sprintf(output, "enemy defeated!            ");
                output_message();
            }else {
                gotoxy(0,info_row);
                sprintf(output, "block health: %3d      ", health);}
                output_message();
        }
        
        timer=dumb_wait(1000);
    

    if(health < 1) {

        // Fail!
        gotoxy(0,info_row);
        sprintf(output, "enemy defeated you!                  ");
        output_message();
        health = 0;
        timer=dumb_wait(1000);
    }

}


// Move the enemies for a given room
void move_enemies() {

    unsigned char rnd;

    // Enemies starts at 1, 0 = no enemy
    for(i=1;i<enemy_count+1;i++)
    {
        if(enemies[i].room == room && enemies[i].health>0){
            
            enemies[i].old_x = enemies[i].x;
            enemies[i].old_y = enemies[i].y; 
            
            // Rat is random
            if(enemies[i].tile == 'R') {
                rnd = (rand() % (4)) + 1; 
                if(rnd == 4) enemies[i].x-=1;
                if(rnd == 2) enemies[i].x+=1;
                if(rnd == 1) enemies[i].y-=1;
                if(rnd == 3) enemies[i].y+=1;
            }

            // Gobbo goes for player
            if(enemies[i].tile == 'G') {
                if(enemies[i].x > player_x) enemies[i].x-=1;
                if(enemies[i].x < player_x) enemies[i].x+=1;
                if(enemies[i].y > player_y) enemies[i].y-=1;
                if(enemies[i].y < player_y) enemies[i].y+=1;
            }

            // Redraw
            c=get_map(enemies[i].x,enemies[i].y);
            if(c!=32) {
                enemies[i].x = enemies[i].old_x;
                enemies[i].y = enemies[i].old_y;
                if(c==64) enemy_attack(i);
            }else{
                set_map(enemies[i].old_x, enemies[i].old_y, 32);
                cputcxy(enemies[i].old_x, enemies[i].old_y, 32);
            }
            set_map(enemies[i].x, enemies[i].y, enemies[i].tile);
            cputcxy(enemies[i].x, enemies[i].y,enemies[i].tile);
            
        }
    }

}

void draw_screen() {

    int row,col;
    for(row=1; row<22; row++)
    {
        for(col=1; col < 38; col++){
            cputcxy(col,row,get_map(col,row));
        }
    };

}



void draw_momentary_object(unsigned int obj_old_x, unsigned int obj_old_y,unsigned int  obj_x, unsigned int obj_y, unsigned int obj_tile, unsigned int delay) {

    // Replace tile
    cputcxy(obj_old_x,obj_old_y,get_map(obj_old_x,obj_old_y));

    // Draw tile in new location
    cputcxy(obj_x,obj_y,obj_tile); 
   
    // Delay
    timer=dumb_wait(delay);

    // Replace tile again
    cputcxy(obj_x,obj_y,get_map(obj_x,obj_y));
    
}


void draw_move(bool replace) {

    // Delete the player character
    if(!replace) {
        set_map(old_x, old_y, 32);
    }

    // Draw new location
    cputcxy(old_x,old_y,get_map(old_x,old_y));
    cputcxy(player_x, player_y, 64); 
    set_map(player_x, player_y, 64);
}




// This is the default title screen
void title_screen() {
    
    clrscr();
    sprintf(output, "ASCII Dungeon\na game by retrogamecoders.com\nPRESS A KEY");
    output_message();
    key=cgetc();
    in_play=true;
}


bool game_over() {
    clrscr();
    gotoxy(10,10);
    sprintf(output, "game over\n\n");
    output_message();
    timer=dumb_wait(1000);
    gotoxy(0,14);
    sprintf(output, "ah, such a shame,\nyou were doing so well!\n\n");
    output_message();
    timer=dumb_wait(1000);
    gotoxy(10,19);
    sprintf(output, "score:%03d\n\nplay again (y/n)?",score);
    output_message();
    key=cgetc();
    in_play=false;
    if(key=='n') {
        return false;
    } else {
        return true;
    }
}

unsigned char get_key() {
            // Check keys;
        switch (key=cgetc()) 
        { 

            case 'w':
                if(player_y>0) player_y--; 
                break; 
            case 'a': 
                if(player_x>0) player_x--; 
                break; 
            
            case 's': 
                if(player_y<24) player_y++; 
                break; 
            case 'd': 
                if(player_x<39) player_x++; 
                break; 


            case 'A': 
            case 'o':
                if(sword==true) {
                    draw_momentary_object(player_x-1,player_y,player_x-1,player_y,'-',2000); 
                    attack(10,player_x-1,player_y);
                }
                break;     

            case 'D': 
            case 'p':
                if(sword==true) {
                    draw_momentary_object(player_x+1,player_y,player_x+1,player_y,'-',2000); 
                    attack(10,player_x+1,player_y);
                }
                break; 
            case 'f': 

                if(magic > 5) {

                    magic-=5;
                    fx = player_x+direction_x;
                    fy = player_y+direction_y;  

                    c=get_map(fx,fy);
                    while(c==32 && magic > 0) {             
                        draw_momentary_object(fx,fy,fx,fy,'*',200); 
                        magic-=1;
                        fx = fx+direction_x;
                        fy = fy+direction_y;    
                        c=get_map(fx,fy);
                    }

                    attack(10,fx,fy);

                }

                break;            
            case 'Q':
            case 3:
                in_play = false;
                break;
            default:
                // Figure out scan codes 
                /*gotoxy(0,0);
                printf("%c %d",key, key);*/
                break; 
        }

        // debounce
        dumb_wait(100);

        return key;
}

void game_loop() {

    gotoxy(0,24);
    sprintf(output, " k: %02d S: %03d *: %03d score: %04d", keys, health, magic, score);
    output_message();

    // Change direction
    if(player_x != old_x || player_y != old_y) {
        direction_x = player_x-old_x;
        direction_y = player_y-old_y;
    }


    move_enemies();

    // Backup the location
    old_x = player_x;
    old_y = player_y;

    //if(kbhit()) { Remove comment to make more action than turn-based
        
    key = get_key();
    //} Remove comment to make more action than turn-based

    gotoxy(0,info_row);
    sprintf(output, "                              ");
    output_message();

    // Anything in our path?
    obstruction=false;
    c=get_map(player_x,player_y);
    
    // Collision
    switch (c)
    {
        case '#':
            // Wall               
            obstruction=true;
            break;
        

        case 'K': // Key +1
            keys+=1;
            break;

        // Door
        case '+':
            if(keys>0)
            {
                keys-=1;
                score+=5;
                obstruction=false;

            }else{

                // Not enough keys to unlock!
                set_map(player_x, player_y, '-'); // turn into partially open
                health-=10; // lose 10 health
                obstruction=true;
            }
            break;

        case '-': // Partially open door

            if(keys>0)
            {
                keys-=1;
                score+=5;
                obstruction=false;

            }else{
                // Not enough keys to unlock!
                set_map(player_x, player_y, 32);  // turn into fully open
                health-=10;         // lose 10 health
                obstruction=true;
            }
            break;

        case '/': // Sword!
            sword=true;
            break;

        case '$': // Cash money
            score+=5;
            break;

        case '*': // Potion
            score+=15;
            magic+=100;
            break;

        case 'M': // Cash money
            score+=15;
            break;

        case 'H': // Health
            health+=25;
            if(health>100) health=100; // Can't be more than 100%!
            break;


/* Enemies >> */

        case 'G': // Gobbo
            attack(5,player_x,player_y);
            obstruction=true;
            break;

        case 'R': // Rats
            attack(5,player_x,player_y);
            obstruction=true;
            break;

/* ^^ Enemies */

        case 'I': // Idol
            score+=10;
            idols+=1;
            if(idols==room) {
                room+=1;
                load_room();
                draw_screen();
                idols=0;
            }
            break;

        case 0:
        case 64: // Player
            break;
        
        default:
            
            // convert integer to ascii: itoa(c,buffer,10);
            
            if(c!=32) {
                // Figure out what the code is for tile
                gotoxy(0,0);
                sprintf(output, "bumped into ...... %03d",c);
                obstruction=true;
            }
            
            break;
    }

    // If obstructed then bounce
    if(obstruction) {
        player_x=old_x;
        player_y=old_y;
    } else {
        draw_move(false);
    }

    if(health<1) {
        in_play = false;
    }

    
    
}


int main() {

    // Use current time as 
    // seed for random generator 
    srand(1); 

    /* Clear Screen */
    clrscr();

    /* Hide cursor */
    cursor(0);

    // Titles
    title_screen();

    // Start running     
    run=true;
        
    // Should the program end?
    while(run){

        // Initialize if not already running a game
        if (in_play != true) {
            keys=0;
            health=100;
            score=0;
            keys=0;
            room=1;
            potion=0;
            magic=0;
            enemy_count=0;
            sword=false;
        }

        // Set up the screen
        load_room();
        draw_screen();
        cputcxy(player_x,player_y,'@');

        // Game on!
        in_play = true;

        /* Loop until game ends */
        while (in_play)
        {
            game_loop();
        }

        // Try again or quit?
        run=game_over();

    }
    clrscr();
    sprintf(output, "goodbye!\n\n");
    output_message();
    cursor(1);
    return(0);
}
