#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"
#include <string.h>





int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);
int player_driver(int, int, int, int);  //x1, y1, x2, y2
int rectangle_driver(int, int , int, int, char); //x1, y1, width, height, color
int char_bp_driver(int, int, int, char);  //x1, y1, character_sel, color
int half_player_driver(int, int, int);  //x, y, player_num

//tile drawing functions
void draw_tile_A(int ,int);
void draw_tile_B(int ,int);
void draw_tile_C(int ,int);
void draw_tile_D(int ,int);
void draw_tile_E(int ,int);

// other display functions
void clear_display();

int convert_char(char);
void draw_string(int, int, char *, char);
void draw_string_center_x(int, char *, char);
void draw_complete_screen(int, int);

//player and tile type define
#define PLAYER1 -1
#define PLAYER2 -2
#define STATUSBAR 0
#define TILE_A  1
#define TILE_B  2
#define TILE_C  3
#define TILE_D  4
#define TILE_E  5

#define STATUSBARHEIGHT 3
#define TILESIZE 10
#define MENUOFFSET 100
#define MENUSIZE 5
#define SCREENWIDTH 320
#define SCREENHEIGHT 240
#define MAXBLOCKS 3
#define GAMEHEIGHT 207
#define CHARTOINT 87
#define MAXDIGITS 10
#define CHARWIDTH 16
#define CHARHEIGHT 24

void clear_display() {
    player_driver(320, 240, 320, 240);
    rectangle_driver(0,0,320,240,0x0);
}

void renderTiles(int tile_arr[][3], int arr_len){
    int tile_type;
    int tile_x;
    int tile_y;
    for (int i = 0; i < arr_len; i++)
    {
        tile_x = tile_arr[i][0];
        tile_y = tile_arr[i][1];
        tile_type = tile_arr[i][2];

        switch (tile_type)
        {
        case PLAYER1: 
            half_player_driver(tile_x, tile_y, 1);
            break;

        case PLAYER2:
            half_player_driver(tile_x, tile_y, 2);
            break;

        case TILE_A:
            draw_tile_A(tile_x, tile_y);
            break;

        case TILE_B:
            draw_tile_B(tile_x, tile_y);
            break;

        case TILE_C:
            draw_tile_C(tile_x, tile_y);
            break;

        case TILE_D:
            draw_tile_D(tile_x, tile_y);
            break;

        case TILE_E:
            draw_tile_E(tile_x, tile_y);
            break;

        default:
            //do nothing is tile_type is 0 or out of bound
            break;
        }
    }
    
}

//tile drawing procedure functions

void draw_tile_A(int x, int y){
    //define tile drawing procedure here
    rectangle_driver(x, y, TILESIZE, TILESIZE, 0xAA);
}

void draw_tile_B(int x, int y){
    //define tile drawing procedure here
    rectangle_driver(x, y, TILESIZE, TILESIZE, 0xE0);
}

void draw_tile_C(int x, int y){
    //define tile drawing procedure here
    rectangle_driver(x, y, TILESIZE, TILESIZE, 0x03);
}

void draw_tile_D(int x, int y){
    //define tile drawing procedure here
    rectangle_driver(x, y, TILESIZE, TILESIZE, 0xDD);
}

void draw_tile_E(int x, int y){
    //define tile drawing procedure here
    rectangle_driver(x, y, TILESIZE, TILESIZE, 0x38);
}

//Main for testing purposes
int main(void)
{
    draw_complete_screen(1232, 9999);

    return 0;
}

int convert_char(char c) {
    int x = 0;
    if (c >= 97 && c <= 122) {
        x = c - CHARTOINT;
    }
    else if (c >= 48 && c <= 57) {
        x = c - 48;
    }
    else if (c == '!') {
        x = c + 5;
    }
    else if (c == ':') {
        x = c - 21;
    }
    else if (c == '.') {
        x = c - 10;
    }
    
    return x;
}

void draw_string_center_x(int y1, char * string, char colour) {
    int x = (SCREENWIDTH - strlen(string) * CHARWIDTH) / 2;
    draw_string(x, y1, string, colour);
}

void draw_string(int x1, int y1, char * string, char colour) {
    int char_width = CHARWIDTH; //changeable for bigger spacing
    for(int i = 0; i < strlen(string); i++) {
        if (string[i] != ' ') {
            char_bp_driver(x1 + char_width * i, y1, convert_char(string[i]), colour);
        }
    }
}

void draw_complete_screen(int time_finished, int blocks_placed) {
    clear_display();
    int y = (SCREENHEIGHT - CHARHEIGHT * 3) / 2;
    
    char message[] = "level completed";

    int message_length_timer = 6;
    int time_finished_copy = time_finished;
    while (time_finished_copy > 0) { // used too divide up numbers larger than 10 into individual digits
        message_length_timer++;
        time_finished_copy /= 10;
    }
    char time[message_length_timer];
    sprintf(time, "time: %d", time_finished);
    
    int message_length_blocks = 8;
    int blocks_placed_copy = blocks_placed;
    while (blocks_placed_copy > 0) { // used too divide up numbers larger than 10 into individual digits
        message_length_blocks++;
        blocks_placed_copy /= 10;
    }
    char blocks[message_length_blocks];
    sprintf(blocks, "blocks: %d", blocks_placed);

    draw_string_center_x(y, message, 0xFF);
    draw_string_center_x(y + CHARHEIGHT, time, 0xFF);
    draw_string_center_x(y + CHARHEIGHT * 2, blocks, 0xFF);

}


/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////
/////////////////////////////////Driver Codes (should not need to touch) ////////////////////////////////


int player_driver(int x1, int y1, int x2, int y2)
{
   volatile int * point1_ptr;   // virtual address pointer to obtain x1,y1
   volatile int * point2_ptr;   // virtual address pointer to obtain x2,y2

   int fd = -1;               // used to open /dev/mem for access to physical addresses
   void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   point1_ptr = (unsigned int *) (LW_virtual + 0);
   point2_ptr = (unsigned int *) (LW_virtual + 4);

   *point1_ptr = y1*512 + x1;  //y1 [16:9] x1 [8:0]
   *point2_ptr = y2*512 + x2;
    
   unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
   close_physical (fd);   // close /dev/mem
   return 0;
}

int half_player_driver(int x, int y, int player_num){
    volatile int * point1_ptr;   // virtual address pointer to obtain x1,y1

    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
        
    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical (fd)) == -1)
        return (-1);
    if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
        return (-1);

    // Set virtual address pointer to I/O port
    if(player_num == 1){
        point1_ptr = (unsigned int *) (LW_virtual + 0);
    }
    else{
        point1_ptr = (unsigned int *) (LW_virtual + 4);
    }

    *point1_ptr = y*512 + x;  //y1 [16:9] x1 [8:0]
        
    unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
    close_physical (fd);   // close /dev/mem
    return 0;
}

int rectangle_driver(int x1, int y1, int width, int height, char color)
{
   volatile int * control_ptr;   // virtual address pointer to obtain x1,y1

    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical (fd)) == -1)
      return (-1);
    if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

    control_ptr = control_ptr = (unsigned int *) (LW_virtual + 1024);
    while(*control_ptr == 0){
        //read ready state of fsm, hold until ready
    }

   // Set virtual address pointer to I/O port
   //write the x, y coordinate and color into the first control register at word 0x2  offset of module = 0x400 = 1024
   control_ptr = (unsigned int *) (LW_virtual + 8 + 1024);
   *control_ptr = color * 131072 + y1 * 512 + x1;

   //write the width and height into second control register at word 0x3
   control_ptr = (unsigned int *) (LW_virtual + 12 + 1024);
   *control_ptr = height * 512 + width;

   //write mode 0x0 and start flag into word 0x1
   control_ptr = (unsigned int *) (LW_virtual + 4 + 1024);
   *control_ptr = 1;

   //reset start flag
   control_ptr = (unsigned int *) (LW_virtual + 4 + 1024);
   *control_ptr = 0;
    
   unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
   close_physical (fd);   // close /dev/mem
   return 0;
}


int char_bp_driver(int x1, int y1, int char_sel, char color)
{
   volatile int * control_ptr;   // virtual address pointer to obtain x1,y1

   int fd = -1;               // used to open /dev/mem for access to physical addresses
   void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

    control_ptr = control_ptr = (unsigned int *) (LW_virtual + 1024);
    while(*control_ptr == 0){
        //constantly read word 0x0 which reads the ready flag
        //read ready state of fsm, hold until ready flag = 1'b1
    }

   //write the x, y coordinate and color into the first control register at word 0x2  offset of module = 0x400 = 1024
   control_ptr = (unsigned int *) (LW_virtual + 8 + 1024);
   *control_ptr = color * 131072 + y1 * 512 + x1;

   //write the commanded character select into second control register at word 0x3
   control_ptr = (unsigned int *) (LW_virtual + 12 + 1024);
   *control_ptr = char_sel;

   //write mode of 0x1 and start flag into word 0x1
   control_ptr = (unsigned int *) (LW_virtual + 4 + 1024);
   *control_ptr = 1 * 2 + 1;

   //reset start flag
   control_ptr = (unsigned int *) (LW_virtual + 4 + 1024);
   *control_ptr = 1 * 2 + 0;
    
   unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
   close_physical (fd);   // close /dev/mem
   return 0;
}



/////////////// code for mapping and unmapp virtual memory //////////////////

// Open /dev/mem, if not already done, to give access to physical addresses
int open_physical (int fd)
{
   if (fd == -1)
      if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1)
      {
         printf ("ERROR: could not open \"/dev/mem\"...\n");
         return (-1);
      }
   return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical (int fd)
{
   close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
   void *virtual_base;

   // Get a mapping from physical addresses to virtual addresses
   virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
   if (virtual_base == MAP_FAILED)
   {
      printf ("ERROR: mmap() failed...\n");
      close (fd);
      return (NULL);
   }
   return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void * virtual_base, unsigned int span)
{
   if (munmap (virtual_base, span) != 0)
   {
      printf ("ERROR: munmap() failed...\n");
      return (-1);
   }
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
