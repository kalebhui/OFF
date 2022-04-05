#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"





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
void draw_status_bar(int, int);

// other display functions
void clear_display();

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
#define SCREENWIDTH 320
#define SCREENHEIGHT 240

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

        case STATUSBAR:
            draw_status_bar(tile_x, tile_y);

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

void draw_status_bar(int x, int y){
    //define tile drawing procedure here
    rectangle_driver(x, y, SCREENWIDTH, STATUSBARHEIGHT, 0xFF);
}

void draw_tile_A(int x, int y, int tileSize){ //Normal
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0xDD);
}

void draw_tile_B(int x, int y, int tileSize){ //Red
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0xE0);
}

void draw_tile_C(int x, int y, int tileSize){ //Ice
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0x1F);
    rectangle_driver(x+4, y+0, 2, 2, 0xFF);
    rectangle_driver(x+2, y+2, 2, 2, 0xFF);
    rectangle_driver(x+0, y+4, 2, 2, 0xFF);
    rectangle_driver(x+6, y+4, 2, 2, 0xFF);
    rectangle_driver(x+8, y+2, 2, 2, 0xFF);
    rectangle_driver(x+4, y+6, 2, 2, 0xFF);
    rectangle_driver(x+2, y+8, 2, 2, 0xFF);
}

void draw_tile_D(int x, int y, int tileSize){  //trap
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, 3, 0xE0);
    rectangle_driver(x, y+3, tileSize, tileSize-3, 0xDD);
}

void draw_tile_E(int x, int y, int tileSize){  //Finish
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0x38);
}

//Main for testing purposes
int main(void)
{
    clear_display();
    // sample mains
    int coords[18][3] = {
        {10, 190, -1},
        {300, 30, -2},
        {0, 210, 0},
        {290, 40, 5},
        {300, 40, 5},
        {310, 40, 5},
        {0, 200, 2},
        {10, 200, 2},
        {20, 200, 2},
        {50,160,1},
        {60,160,1},
        {70,160,1},
        {100,100,3},
        {100,110,3},
        {100,120,3},
        {200,100,4},
        {210,90,4},
        {220,80,4}
    };


    renderTiles(coords, 18);

    return 0;
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