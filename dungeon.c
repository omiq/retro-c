#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <conio.h>

#ifdef __CBM__
    #include <cbm.h>
    #include <peekpoke.h>
#endif

// Global key variable
bool run=true;
bool in_play=false;
bool obstruction=false;
unsigned int timer;
unsigned int key,i,c;
unsigned char x=19;
unsigned char y=8;
unsigned char old_x, old_y;
unsigned char room=0;
unsigned char buffer [sizeof(int)*8+1];

// Player 
unsigned char keys,idols,potion=0;
int health=100;
int magic=0;
unsigned int score=0;

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
 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 32,102, 32, 32, 32, 32, 32, 32, 32, 32, 88, 32, 32, 32, 32,  9, 32, 32, 32, 32, 38, 32, 32,102, 32, 32, 32, 32, 32, 32,
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
        if(c==0) {
            y=pos/40;
            x=pos % 40;
        }  
        game_map[pos] = c;     
    }
    //printf("..done!");      
}

void set_map(char x, char y, int tile) {
    
    // Set the part of the array to the given tile
    game_map[40*y+x]=tile;
    
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


void draw_momentary_object(obj_old_x, obj_old_y, obj_x, obj_y, obj_tile) {

    // Replace tile
    cputcxy(obj_old_x,obj_old_y,map(obj_old_x,obj_old_y));

    // Draw tile in new location
    cputcxy(obj_x,obj_y,obj_tile); 

    // Delay
    timer=dumb_wait(2000);

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

void game_loop() {

    gotoxy(0,24);
    printf(" keys %02d health %03d magic %03d score %03d", keys, health, magic, score);

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
            draw_momentary_object(x-1,y,x-1,y,131); 
            break;     
        case 's': 
            if(y<24) y++; 
            break; 
        case 'd': 
            if(x<39) x++; 
            break; 
        case 'D': 
            draw_momentary_object(x+1,y,x+1,y,31); 
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
            // Not enough keys to unlock!
            set_map(x, y, 32); // turn into fully open
            health-=10; // lose 10 health
            obstruction=true;
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
            idols+=1;
            if(idols==2) {
                room+=1;
                load_room();
                draw_screen();
                idols=0;
            }
            break;
        
        default:
//          itoa(c,buffer,10);
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

    // Initialize
    run=true;
    keys=0;
    health=100;
    score=0;
    keys=0;
    room=0;
    potion=0;
    magic=0;

    // Should the program end?
    while(run){

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
