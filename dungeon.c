/*

 cl65 -t c64 -v -o dungeon64.prg dungeon.c && x64sc dungeon64.prg > /dev/null
 cl65 -t c64 -v -o dungeon64.prg dungeon.c && x64sc dungeon64.prg > /dev/null

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>


#if defined (__CC65__)
#include<cc65.h>
#include <conio.h>

#if defined __CBM__
    #include <cbm.h>
    #include <peekpoke.h>
#endif
#endif

// Global key variable
bool run=true;
bool in_play=false;
bool obstruction=false;
unsigned int timer,delay;
unsigned int key,i,c;
unsigned char x=19;
unsigned char y=8;
unsigned char old_x, old_y, direction_x, direction_y, fx, fy;
unsigned char room=0;
unsigned char buffer [sizeof(int)*8+1];

// Player 
unsigned char keys,idols,potion=0;
int health=100;
int magic=0;
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
    unsigned char health;
    unsigned char strength;
    unsigned char speed;
    unsigned char armour;
};
unsigned int this_enemy = 0;

struct enemy enemies[1000];



unsigned char rooms[] = {
// Room 1
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 0, 32, 32, 32, 32, 32, 11, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 94, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
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
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 30, 32, 88, 32, 32, 32, 32,  9, 32, 32, 32, 32, 38, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 94, 32, 32, 32, 32,102, 32, 32, 94, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 36, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32,102,102,102,102,102,102,102,102,102,102,102, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32
,// room 2
 
 105,120,120,120,120,120,120,120, 66, 61, 61, 66,120,120,120,120,120,120,120, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 97, 32, 32, 32, 32, 32, 32, 32, 66, 61, 61, 66, 32, 32, 32, 32, 32, 32, 32, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 97, 32, 32, 32, 32, 32, 32, 32, 66, 61, 61, 66, 32, 32, 32, 32, 32, 32, 32, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
117, 32, 32, 32, 32, 32, 32, 32, 66, 61, 61, 66, 32, 32, 32, 32, 32, 32, 32, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 97, 32, 32, 32, 32, 32, 32, 32, 32,  0, 32, 32, 32, 32, 32, 32, 32, 32, 32,116, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97,121, 98, 98, 98, 98, 98, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 97, 32, 32, 90, 32, 94, 94, 94, 94, 94, 94, 94, 94, 94, 32, 32, 32, 32, 32, 97, 97, 32, 32, 32, 32, 32,127, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98,117,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,117,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97,
117, 32, 32, 32, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,116, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 88, 32, 32, 32, 32, 32, 32, 32, 97,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 38, 32, 32, 90, 32, 32, 32, 32, 97, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 83, 32, 32, 32, 32, 32, 32, 32, 32, 97,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 83, 32, 32, 32, 32, 32, 32, 32, 32, 97, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 38, 32, 32, 90, 32, 32, 32, 32, 97,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 88, 32, 32, 32, 32, 32, 32, 97,117, 32, 32, 32, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,116,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97,
 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,117, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97,
 97, 32, 32, 32, 32, 32, 32, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98,117, 97, 32, 32, 90, 32, 94, 94, 94, 94, 94, 94, 94, 94, 94, 32, 32, 32, 32, 32, 97,
127, 98, 98, 98, 98, 98, 98,117, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,116,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,117, 32, 32, 32, 32, 32, 32, 32, 66, 61, 61, 66, 32, 32, 32, 32, 32, 32, 32, 97,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97, 32, 32, 32, 32, 32, 32, 32, 66, 61, 61, 66, 32, 32, 32, 32, 32, 32, 32, 97,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 97, 32, 32, 32, 32, 32, 32, 32, 66, 61, 61, 66, 32, 32, 32, 32, 32, 32, 32, 97,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,105,120,120,120,120,120,120,120, 66, 61, 61, 66,120,120,120,120,120,120,120, 97,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32
 
 // room 3
 ,
  
   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 87, 87, 87, 87, 32, 87, 87, 87, 87, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 87, 87, 32, 32, 87, 87, 87, 87, 32, 32, 32, 32, 87, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 87, 32, 32, 32, 32, 32, 32,
 32, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32, 32, 32, 32, 32, 88, 83, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32, 32, 32,
 32, 87, 32, 32, 32, 88, 32, 32, 32, 32, 32, 32, 32, 32, 32,  9, 32, 32, 32, 32, 32, 32, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 87, 32, 87, 32, 32, 32, 32, 32,
 32, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 38, 32, 87, 87, 32, 32, 38, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 32, 87, 87, 32, 87, 87, 32, 32, 32, 32,
 32, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 32, 32, 32, 87, 32, 32, 32, 32,
 32, 87, 32, 32, 32, 32, 32, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32,
 32, 87, 87, 32, 32, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32, 32, 32, 32, 32, 87, 45, 87, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32, 32,
 32, 32, 87, 32, 32, 32, 38, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32, 32, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32,
 32, 32, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 32, 87, 87, 32, 32, 32, 94, 94, 94, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32,
 32, 32, 32, 87, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 32, 32, 94, 94, 94, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32,
 32, 32, 32, 32, 32, 87, 87, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,  9, 32, 32, 32, 94, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 87, 87, 87, 87, 87, 32, 32, 32, 32, 32, 87, 32, 32, 94, 32, 32, 32, 32, 32, 32,  0, 32, 32, 32, 32, 32, 87, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 87, 87, 87, 87, 87, 32, 94, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 87, 32, 32, 32, 32, 32, 87, 87, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 87, 87, 87, 87, 87, 87, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32

};

unsigned char game_map[1000];

unsigned int map(char x, char y) {
    
    unsigned char c;

    c = game_map[40*y+x];
    if(c>64) c+=64;
    return c;
    
    // For testing you could use the following to output the charset
    // return 40*y+x;
}

void load_room() {
    int pos;

    clrscr();
    gotoxy(0,0);
    printf("loading room %d",room+1);

    for (pos = 0; pos < 1000; pos++) {   

        c=rooms[pos+(1000*room)];

        // Player x and y
        if(c==0) {
            y=pos/40;
            x=pos % 40;
        }

        // Goblin
        if(c==38) {

            // Increment for next enemy (Enemy 0 is counted as no enemy)
            enemy_count+=1;

            // Create the enemy in the list
            enemies[enemy_count].tile = c;
            enemies[enemy_count].room = room;
            enemies[enemy_count].x = pos % 40;
            enemies[enemy_count].y = pos/40;
            enemies[enemy_count].old_x = enemies[enemy_count].x;
            enemies[enemy_count].old_y = enemies[enemy_count].y;
            enemies[enemy_count].health = 30;
            enemies[enemy_count].strength = 5;
            enemies[enemy_count].speed = 1;
            enemies[enemy_count].armour = 10;

        }  

        // Rat
        else if (c==94) {

            // Increment for next enemy (Enemy 0 is counted as no enemy)
            enemy_count+=1;

            // Create the enemy in the list
            enemies[enemy_count].tile = 158;
            enemies[enemy_count].room = room;
            enemies[enemy_count].x = pos % 40;
            enemies[enemy_count].y = pos/40;
            enemies[enemy_count].old_x = enemies[enemy_count].x;
            enemies[enemy_count].old_y = enemies[enemy_count].y;
            enemies[enemy_count].health = 15;
            enemies[enemy_count].strength = 1;
            enemies[enemy_count].speed = 2;
            enemies[enemy_count].armour = 0;

        }  

        game_map[pos] = c;     
    }
    
    /*
    printf("\n\nenemies %3d\n", enemy_count);
    for(i=1; i<enemy_count+1;i++)
    {
        printf("%s %3d\n",enemies[i].tile,i);
    }

    key = cgetc();      
    */
}

void set_map(char x, char y, int tile) {
    
    // Set the part of the array to the given tile
    game_map[40*y+x]=tile;
    
}

// Returns the enemy for a given x,y coord
unsigned int which_enemy(ex,ey) {

    if(map(ex,ey)==32) return 0;

    // Enemies starts at 1, 0 = no enemy
    for(i=1;i<enemy_count+1;i++)
    {
        if(enemies[i].x == ex && enemies[i].y == ey) return i;
    }

    // No enemies
    return 0;

}

void draw_screen() {

    int row,col;
    for(row=0; row<25; row++)
    {
        for(col=0; col < 40; col++){
            cputcxy(col,row,map(col,row));
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


unsigned int dumb_wait(unsigned int delay) {
    for(timer=0; timer<delay; timer++) {}
    return timer;
}


void draw_momentary_object(obj_old_x, obj_old_y, obj_x, obj_y, obj_tile, delay) {

    // Replace tile
    cputcxy(obj_old_x,obj_old_y,map(obj_old_x,obj_old_y));

    // Draw tile in new location
    cputcxy(obj_x,obj_y,obj_tile); 

    // Delay
    timer=dumb_wait(delay);

    // Replace tile again
    cputcxy(obj_x,obj_y,map(obj_x,obj_y));
    
}


void draw_move(bool replace) {

    // Delete the player character
    if(!replace) {
        set_map(old_x, old_y, 32);
    }

    // Draw new location
    cputcxy(old_x,old_y,map(old_x,old_y));
    cputcxy(x,y,64); 
    
}

void title_screen() {
    clrscr();
    printf("a game by retrogamecoders.com\n\npress a key");
    key=cgetc();
    in_play=true;
}

bool game_over() {
    clrscr();
    printf("game over\n\nah, such a shame,\nyou were doing so well!\n\nscore:%03d\n\nplay again (y/n)?",score);
    key=cgetc();
    in_play=false;
    if(key=='n') {
        return false;
    } else {
        return true;
    }
}

void attack(weapon, ax, ay)
{
    int rnum = 0;
    this_enemy = 0;
    this_enemy = which_enemy(ax,ay);
    if(this_enemy == 0) {
        return;
    } 

    rnum = (rand() % (20 - 1 + 1)) + 1; 
    gotoxy(0,0);
    if(rnum > enemies[this_enemy].armour+enemies[this_enemy].speed) {

        // Damage!
        if(enemies[this_enemy].health<weapon) 
        {
            enemies[this_enemy].health = 0;
        }
        else
        {
            enemies[this_enemy].health-=weapon;
            printf("hit!! enemy health: %3d", enemies[this_enemy].health);
        }
       
        
    } else {
        printf("miss! enemy health: %3d", enemies[this_enemy].health);
        if((x == ax && y == ay)||(x == ax && (y == ay + 1 || y == ay - 1)) || (y == ay && (x == ax + 1 || x == ax - 1))) 
        {
            health -= enemies[this_enemy].strength;
        }
    }

    if(enemies[this_enemy].health==0) {

        // Success!
        gotoxy(0,0);
        printf("enemy defeated!                      ");

        // Draw tile in new location
        cputcxy(ax,ay,32); 
        set_map(ax,ay,32);

        // Up the score
        score+=10;
    }

}

void game_loop() {

    gotoxy(0,24);
    printf(" keys %02d health %03d magic %03d score %03d", keys, health, magic, score);

    // Change direction
    if(x != old_x || y != old_y) {
        direction_x = x-old_x;
        direction_y = y-old_y;
    }

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
        case 'A': 
            if(sword==true) {
                draw_momentary_object(x-1,y,x-1,y,131,2000); 
                attack(10,x-1,y);
            }
            break;     
        case 's': 
            if(y<24) y++; 
            break; 
        case 'd': 
            if(x<39) x++; 
            break; 
        case 'D': 
            if(sword==true) {
                draw_momentary_object(x+1,y,x+1,y,31,2000); 
                attack(10,x+1,y);
            }
            break; 
        case 'f': 

            if(magic > 5) {

                magic-=5;
                fx = x+direction_x;
                fy = y+direction_y;  

                c=map(fx,fy);
                while(c==32 && magic > 0) {             
                    draw_momentary_object(fx,fy,fx,fy,'*',200); 
                    magic-=1;
                    fx = fx+direction_x;
                    fy = fy+direction_y;  
                    c=map(fx,fy);
                }

                attack(10,fx,fy);

              }

            break;            
        case 'Q':
            in_play = false;
            break;
        default: 
            break; 
    }

    // Anything in our path?
    obstruction=false;
    c=map(x,y);
    
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
                set_map(x, y, 28); // turn into partially open
                health-=10; // lose 10 health
                obstruction=true;
            }
            break;

        case 28: // Partially open door

            if(keys>0)
            {
                keys-=1;
                score+=10;
                obstruction=false;

            }else{
                // Not enough keys to unlock!
                set_map(x, y, 32);  // turn into fully open
                health-=10;         // lose 10 health
                obstruction=true;
            }
            break;

        case 30: // Sword!
            sword=true;
            break;

        case 36: // Cash money
            score+=50;
            break;

        case 145: // Potion
            score+=150;
            magic+=100;
            break;

        case 154: // Cash money
            score+=150;
            break;

        case 147: // Health
            health+=25;
            break;


/* Enemies >> */

        case 38: // Gobbo
            attack(5,x,y);
            obstruction=true;
            break;

        case 158: // Rats
            attack(5,x,y);
            obstruction=true;
            break;

/* ^^ Enemies */

        case 152: // Idol
            score+=100;
            idols+=1;
            if(idols==2) {
                room+=1;
                load_room();
                draw_screen();
                idols=0;
            }
            break;
        
        default:
            
            // convert integer to ascii: itoa(c,buffer,10);
            
            if(c!=32) {
                gotoxy(0,0);
                printf("%03d",c);
                obstruction=true;
            }
            
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
        in_play = false;
    }
    
}


int main() {

    // Use current time as 
    // seed for random generator 
    srand(1); 

    /* Clear Screen */
    clrscr();

#ifdef __PET__
    // Uppercase/Graphical characterset = 12
    POKE(59468,12);
#endif

#ifdef __C64__
    POKE(53272,21);
#endif

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
            room=0;
            potion=0;
            magic=0;
            enemy_count=0;
        }

        // Set up the screen
        load_room();
        draw_screen();
        cputcxy(x,y,'@');

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
    printf("goodbye!\n\n");
    cursor(1);
    return(0);
}
