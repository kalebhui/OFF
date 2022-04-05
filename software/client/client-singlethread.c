#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>  
#include <fcntl.h>   
#include <errno.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <signal.h> 
#include <limits.h>   
#include <linux/input.h> 
#include <time.h>
#include <sys/mman.h>
#include "address_map_arm.h"

#define KBD_EVENT "/dev/input/event0"

// define port we are connecting to
#define PORT 3389

// Use eth0 gateway address
#define ipaddress "35.212.170.255"
#define buffer_size 7000
#define bitmap_width 40
#define bitmap_height 24

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

int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);
int player_driver(int, int, int, int);  //x1, y1, x2, y2
int rectangle_driver(int, int, int, int, char); //x1, y1, width, height, color
int char_bp_driver(int, int, int, char);  //x1, y1, character_sel, color
int half_player_driver(int, int, int);  //x, y, player_num

// tile drawing functions
void draw_status_bar(int, int);
void draw_tile_A(int, int, int);
void draw_tile_B(int, int, int);
void draw_tile_C(int, int, int);
void draw_tile_D(int, int, int);
void draw_tile_E(int, int, int);

// other display functions
void clear_display();

// keyboard functions
char get_keyvalue(int kbd_ptr);
void * listen_kbd(void * args);

// client-side socket functions
void *game_handler(void *arg);
int create_socket();

// messages
int convert_char(char);
void draw_string(int, int, char *, char);
// void disconnect_message();!!!!

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Render Functions ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

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
            break;

        case TILE_A:
            draw_tile_A(tile_x, tile_y, TILESIZE);
            break;

        case TILE_B:
            draw_tile_B(tile_x, tile_y, TILESIZE);
            break;

        case TILE_C:
            draw_tile_C(tile_x, tile_y, TILESIZE);
            break;

        case TILE_D:
            draw_tile_D(tile_x, tile_y, TILESIZE);
            break;

        case TILE_E:
            draw_tile_E(tile_x, tile_y, TILESIZE);
            break;

        case TILE_A + MENUOFFSET:
            draw_tile_A(tile_x, tile_y, MENUSIZE);
            break;

        case TILE_B + MENUOFFSET:
            draw_tile_B(tile_x, tile_y, MENUSIZE);
            break;

        case TILE_C + MENUOFFSET:
            draw_tile_C(tile_x, tile_y, MENUSIZE);
            break;

        case TILE_D + MENUOFFSET:
            draw_tile_D(tile_x, tile_y, MENUSIZE);
            break;
            
        case TILE_E + MENUOFFSET:
            draw_tile_E(tile_x, tile_y, MENUSIZE);
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

void draw_tile_A(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0xAA);
}

void draw_tile_B(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0xE0);
}

void draw_tile_C(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0x03);
}

void draw_tile_D(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0xDD);
}

void draw_tile_E(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0x38);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Message Code ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int convert_char(char c) {
    int x = c - CHARTOINT;
    return c - CHARTOINT;
}

void draw_string(int x1, int y1, char * string, char colour) {
    int char_width = 16; //changeable for bigger spacing
    for(int i = 0; i < strlen(string); i++) {
        if (string[i] != ' ') {
            char_bp_driver(x1 + char_width * i, y1, convert_char(string[i]), colour);
        }
    }
}

void draw_status_text(int num[], int length, char colour) {
    int x1 = 0;
    int block_offset = SCREENWIDTH / length;
    int char_width = 16;
    int reverse[MAXDIGITS];
    for(int i = 0; i < length; i++) {
        int num_digits = 0;
        while (num[i] > 0) { // used too divide up numbers larger than 10 into individual digits
            reverse[num_digits] = num[i] % 10;
            num_digits++;
            num[i] /= 10;
        }
        if (num_digits == 0) { // if there are no more blocks display zero
            char_bp_driver(block_offset * i + block_offset / 2 + TILESIZE, GAMEHEIGHT + 3 + STATUSBARHEIGHT, 0, colour);
        }
        else {
            int count = 0;
            for (int j = num_digits - 1; j >= 0; j--) {
                char_bp_driver(block_offset * i + block_offset / 2 + char_width * count + TILESIZE, 
                                GAMEHEIGHT + 3 + STATUSBARHEIGHT, reverse[j], colour);
                count++;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Driver Code ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////
/////////////// code for mapping and unmapp virtual memory //////////////////
/////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////
////////////////////////////Keyboard Drivers//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

char cur_key = '/';

void * listen_kbd(void * args){
    int* kbd_ptr;
    int fd = -1;
    struct input_event *kbd_event;
    int ret = 0;

    fd = open(KBD_EVENT, O_RDONLY);
    if(fd < 0){
        printf("open file %s failed\n", KBD_EVENT);
        return NULL;
    }
    
    while (1) {
        kbd_event = malloc(sizeof(struct input_event));
        memset(kbd_event, 0, sizeof(struct input_event));  //clear/reset kbd_event
        ret = read(fd, kbd_event, sizeof(struct input_event));

        if(ret != sizeof(struct input_event)){
            printf("read file %s failed with error %d\n", KBD_EVENT, ret);
        }

        if(kbd_event->type == 0x01){
            if (kbd_event->value == 1) {
                cur_key = get_keyvalue(kbd_event->code);
            }
            else if (kbd_event->value == 0) {
                cur_key = '/';
            }
        }
    }

    close(fd);
    return NULL;
}

char get_keyvalue(int kbd_ptr){
    char key_value;
    switch(kbd_ptr){
        case 0x7001a:
            key_value = 'w';
            break;
        case 0x11:
            key_value = 'w';
            break;
        case 0x70004:
            key_value = 'a';
            break;
        case 0x1e:
            key_value = 'a';
            break;
        case 0x70016:
            key_value = 's';
            break;
        case 0x1f:
            key_value = 's';
            break;
        case 0x70007:
            key_value = 'd';
            break;
        case 0x20:
            key_value = 'd';
            break;
        case 0x13:
            key_value = 'r';
            break;
        case 0x7002c:
            key_value = 'p'; //space
            break;
        case 0x39:
            key_value = 'p';
            break;
        case 0x70027:
            key_value = '0';
            break;
        case 0xb:
            key_value = '0';
            break;
        case 0x7001e:
            key_value = '1';
            break;
        case 0x02:
            key_value = '1';
            break;
        case 0x7001f:
            key_value = '2';
            break;
        case 0x03:
            key_value = '2';
            break;
        case 0x70020:
            key_value = '3';
            break;
        case 0x04:
            key_value = '3';
            break;
        case 0x70021:
            key_value = '4';
            break;
        case 0x05:
            key_value = '4';
            break;
        case 0x70022:
            key_value = '5';
            break;
        case 0x06:
            key_value = '5';
            break;
        case 0x70023:
            key_value = '6';
            break;
        case 0x07:
            key_value = '6';
            break;
        case 0x70024:
            key_value = '7';
            break;
        case 0x08:
            key_value = '7';
            break;
        case 0x70025:
            key_value = '8';
            break;
        case 0x09:
            key_value = '8';
            break;
        case 0x70026:
            key_value = '9';
            break;
        case 0xa:
            key_value = '9';
            break;
        case 0x70028:
            key_value = 'e'; //enter
            break;
        case 0x1c:
            key_value = 'e';
            break;
        default:
            key_value = '/';
            // default statements
        }
    return key_value;
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////Socket Communication///////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int create_socket() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET; // For IPv6
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ipaddress, &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to the socket
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    return sock;
}

void *game_handler(void *arg) {
    int sock1 = create_socket(); // sender
    int sock2 = create_socket(); // receiver
    int kbd_ptr;
    int ret;
    char key_v;
    char * buffer = malloc(buffer_size);
    int valread;

    if(sock1 == -1 || sock2 == -1) {
        printf("connection failed \n");
        return NULL;
    }

	if( send(sock1 , "1", 1, 0) < 0)
	{
		printf("Send failed\n");
		return NULL;
	}

    if( send(sock2 , "0", 1 , 0) < 0) {
        printf("Send failed\n");
        return NULL;
    }

    int coords_row = 0;
    int coords_col = 0;
    int coords[1000][3] = {0};
    int statusCount[MAXBLOCKS] = {0};
    int signal = 1;
    int clear_signal = 0;
    int blockIndex = 0; //signal for data containing block amounts for p2
    int blockSignal = 0;

    while (1) {
        coords_row = 0;
        coords_col = 0;
        if (cur_key != '/') {
            printf("%c\n", cur_key);
            if( send(sock1 , &cur_key , 1 , 0) < 0) {
                printf("Send failed\n");
                return NULL;
            }
            signal = 1;
        }
        else {
            if (signal) {
                printf("No keyboard input\n");
                signal = 0;
            }
        }
        
        valread = read(sock2, buffer, buffer_size);
        // int changed = strcmp(prev_buffer, buffer); 
        char * token = strtok(buffer, ",");
        
        while( token != NULL ) {
            if (*token == '.') {
                clear_signal = 1;
            }
            else if(*token == '&') { // '&' signals end of coordinates
                break;
            }
            else if(*token == '!' && blockSignal) { //If we recieve the second ! we have reached the end of the statusCount
                blockSignal = 0;
            }
            else if(*token == '!' || blockSignal) { 
                if(blockSignal == 0) { // start checking for block numbers
                    blockSignal = 1;
                    blockIndex = 0;
                }
                else {
                    statusCount[blockIndex] = atoi(token);
                    blockIndex++;
                }
            }
            else {
                int num = atoi(token);
                coords[coords_row][coords_col] = num;
                coords_col++;
                if(coords_col == 3) {
                    coords_col = 0;
                    coords_row++;
                }
            }
            token = strtok(NULL, ",");
        }

        if( send(sock2 , "0", 1, 0) < 0) {
            printf("Send failed\n");
            return NULL;
        }
        
        if (clear_signal) { // reset display if there are block changes
            clear_display();
            clear_signal = 0;
        }

        renderTiles(coords, coords_row);
        if (blockIndex) {
            draw_status_text(statusCount, blockIndex, 0xFF);
        }

        usleep(50000); // takes 20 key inputs per second
    }

    return NULL;
}

int main(void)
{
    clear_display();
    pthread_t thread_game;
    pthread_t thread_kb;

    pthread_create(&thread_game, NULL, game_handler, NULL);
    pthread_create(&thread_kb, NULL, listen_kbd, NULL);

    pthread_join(thread_game, NULL);
    printf("thread 1 joined");
    pthread_join(thread_kb, NULL);
    printf("thread 2 joined");

    return 0;
}