#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"


//////////////////////////* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);


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


int main(void)
{
   // sample mains
   //draws a face using rectangle
   rectangle_driver(0,0,320,240, 0xF0);
   rectangle_driver(10,10,300,220,0x00);
   rectangle_driver(50,50,100,100,0x0F);
   rectangle_driver(70,60,10,20,0xFF);
   rectangle_driver(110,60,10,20,0xFF);
   rectangle_driver(80,120,50,10,0xFF);
   
   //draws sample characters using character module
   char_bp_driver(10,50,28,0xF0);
   char_bp_driver(50,50,10,0xFF);
   char_bp_driver(90,50,22,0x0F);
   char_bp_driver(130,50,1,0xFF);
   char_bp_driver(170,50,2,0xFF);
   char_bp_driver(210,50,3,0xFF);
   char_bp_driver(250,50,38,0xFF);

   //moves charater constantly using player module
    int x1,x2,y1,y2;
   for(int i = 0; i<200; i++){
      x1 = i;
      y1 = i;
      x2 = 200-i;
      y2 = 200-i;
      player_driver(x1, y1, x2, y2);
      for(int j = 0; j<500000; j++){}
   }

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