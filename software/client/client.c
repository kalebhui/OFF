/**
 * @file client.c
 * @author Kaleb Hui, Luka Rogic, Sam Gu, Ting Xu
 * @brief Serves as the client to render game state on vga display. File built to run on DE1-SoC HPS.
 * @date 2022-04-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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

// used to connect to keyboard event on usb
#define KBD_EVENT "/dev/input/event0"



// Use eth0 gateway address
#define ipaddress "35.212.170.255"
// define port we are connecting to
#define PORT 3389

// message size from server
#define buffer_size 7000

//player and tile type definitions
#define PLAYER1 -1
#define PLAYER2 -2
#define STATUSBAR 0
#define TILE_REG  1
#define TILE_RED  2
#define TILE_ICE  3
#define TILE_TRAMP  4
#define TILE_FINISH  5
#define TILE_GRAV  6
#define TILE_TNT  7

//game constants 
#define STATUSBARHEIGHT 3
#define TILESIZE 10
#define MENUOFFSET 100
#define MENUSIZE 5
#define SCREENWIDTH 320
#define SCREENHEIGHT 240
#define MAXBLOCKS 10
#define GAMEHEIGHT 207
#define CHARTOINT 87
#define MAXDIGITS 10
#define CHARWIDTH 16
#define CHARHEIGHT 24

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
void draw_tile_reg(int, int, int);
void draw_tile_red(int, int, int);
void draw_tile_ice(int, int, int);
void draw_tile_tramp(int, int, int);
void draw_tile_finish(int, int, int);
void draw_tile_tnt(int, int, int);
void draw_tile_grav(int, int, int);

// keyboard functions
char get_keyvalue(int kbd_ptr);
void * listen_kbd(void * args);

// client-side socket functions
void *game_handler(void *arg);
int create_socket();

// messages
int convert_char(char);
void draw_string(int, int, char *, char);
void draw_string_center_x(int, char *, char);
void draw_complete_screen(int, int);
void draw_post_complete_screen(int);

//draw static backgrounds
void draw_sun(int, int);
void draw_daytime_cloud(int, int);
void draw_daytime_tree(int, int);
void draw_daytime_bg();

void draw_moon(int, int);
void draw_star(int, int);
void draw_night_cloud(int, int);
void draw_night_tree(int, int);
void draw_night_bg();

// other display functions
void clear_display();
void draw_background(int);

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Render Functions ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void draw_background(int mode) {
    if (mode == 1) {
        clear_display();
    }
    else if (mode == 2) {
        draw_daytime_bg();
    }
    else if (mode == 3) {
        draw_night_bg();
    }
}

// draws black square over screen
void clear_display() {
    player_driver(320, 240, 320, 240);
    rectangle_driver(0,0,320,240,0x0);
}

// renders tiles sent from server
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

        case TILE_REG:
            draw_tile_reg(tile_x, tile_y, TILESIZE);
            break;

        case TILE_RED:
            draw_tile_red(tile_x, tile_y, TILESIZE);
            break;

        case TILE_ICE:
            draw_tile_ice(tile_x, tile_y, TILESIZE);
            break;

        case TILE_TRAMP:
            draw_tile_tramp(tile_x, tile_y, TILESIZE);
            break;

        case TILE_FINISH:
            draw_tile_finish(tile_x, tile_y, TILESIZE);
            break;

        case TILE_TNT:
            draw_tile_tnt(tile_x, tile_y, TILESIZE);
            break;

        case TILE_GRAV:
            draw_tile_grav(tile_x, tile_y, TILESIZE);
            break;

        case TILE_REG + MENUOFFSET:
            draw_tile_reg(tile_x, tile_y, MENUSIZE);
            break;

        case TILE_RED + MENUOFFSET:
            draw_tile_red(tile_x, tile_y, MENUSIZE);
            break;

        case TILE_ICE + MENUOFFSET:
            draw_tile_ice(tile_x, tile_y, MENUSIZE);
            break;

        case TILE_TRAMP + MENUOFFSET:
            draw_tile_tramp(tile_x, tile_y, MENUSIZE);
            break;
            
        case TILE_FINISH + MENUOFFSET:
            draw_tile_finish(tile_x, tile_y, MENUSIZE);
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

void draw_tile_reg(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0xDD);
}

void draw_tile_red(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0xE0);
}

void draw_tile_ice(int x, int y, int tileSize){
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

void draw_tile_tramp(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, 3, 0xE0);
    rectangle_driver(x, y+3, tileSize, tileSize-3, 0xDD);
    rectangle_driver(x, y, tileSize, 3, 0xE0);
}

void draw_tile_finish(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0x38);
}

void draw_tile_tnt(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0xE9);
    rectangle_driver(x+2, y+0, 6, tileSize, 0x85);
    rectangle_driver(x+0, y+3, tileSize, 4, 0x25);
    rectangle_driver(x+0, y+3, 3, 1, 0xFF);
    rectangle_driver(x+1, y+4, 1, 3, 0xFF); 
    rectangle_driver(x+3, y+3, 1, 4, 0xFF);
    rectangle_driver(x+4, y+4, 1, 1, 0xFF); 
    rectangle_driver(x+5, y+5, 1, 1, 0xFF);
    rectangle_driver(x+6, y+3, 1, 4, 0xFF); 
    rectangle_driver(x+7, y+3, 3, 1, 0xFF);
    rectangle_driver(x+8, y+4, 1, 3, 0xFF); 
}

void draw_tile_grav(int x, int y, int tileSize){
    //define tile drawing procedure here
    rectangle_driver(x, y, tileSize, tileSize, 0x25);
    rectangle_driver(x+3, y+3, 4, 4, 0xFF);
}

/*draw one sun per time*/
void draw_sun(int x, int y){
    rectangle_driver(x, y, 12, 12, 0xE0);
    rectangle_driver(x+3, y+3, 7, 1, 0xF0);
    rectangle_driver(x+3, y+5, 5, 1, 0xF0);
    rectangle_driver(x+3, y+9, 7, 1, 0xF0);
    rectangle_driver(x+6, y+7, 2, 1, 0xF0);
    rectangle_driver(x+3, y+6, 1, 3, 0xF0);
    rectangle_driver(x+7, y+6, 1, 1, 0xF0);
    rectangle_driver(x+9, y+4, 1, 5, 0xF0);
    for(int i = 0; i < 5; i++){
        rectangle_driver(x-2+i*3, y-3, 2, 2, 0xE0);
        rectangle_driver(x-1+i*3, y-2, 1, 1, 0xF0);
    }
    for(int i = 0; i < 5; i++){
        rectangle_driver(x+i*3, y+13, 2, 2, 0xE0);
        rectangle_driver(x+1+i*3, y+14, 1, 1, 0xF0);
    }
    for(int i = 0; i < 4; i++){
        rectangle_driver(x-3, y+1+i*3, 2, 2, 0xE0);
        rectangle_driver(x-2, y+2+i*3, 1, 1, 0xF0);
    }
    for(int i = 0; i < 4; i++){
        rectangle_driver(x+13, y+i*3, 2, 2, 0xE0);
        rectangle_driver(x+14, y+1+i*3, 1, 1, 0xF0);
    }
}

/*draw one cloud per time*/
void draw_daytime_cloud(int x, int y){
    rectangle_driver(x, y, 5, 1, 0xFF);
    rectangle_driver(x-1, y+1, 8, 2, 0xFF);
    rectangle_driver(x-4, y+3, 14, 4, 0xFF);
    rectangle_driver(x-7, y+7, 19, 2, 0xFF);
    rectangle_driver(x-9, y+9, 23, 6, 0xFF);
    rectangle_driver(x-6, y+15, 15, 1, 0xFF);
    rectangle_driver(x-4, y+12, 1, 2, 0x17);
    rectangle_driver(x+8, y+12, 1, 2, 0x17);
    rectangle_driver(x-2, y+14, 9, 1, 0x17);
}

/*draw one tree per time*/
void draw_daytime_tree(int x, int y){
    rectangle_driver(x, y, 4, 4, 0x55);
    rectangle_driver(x-2, y+4, 8, 4, 0x55);
    rectangle_driver(x-4, y+8, 12, 4, 0x55);
    rectangle_driver(x-6, y+12, 16, 4, 0x55);
    rectangle_driver(x-8, y+16, 20, 4, 0x55);
    rectangle_driver(x-6, y+20, 16, 3, 0x55);
    rectangle_driver(x, y+16, 4, 11, 0x69);
}

/*draw daytime background*/
void draw_daytime_bg(){
    rectangle_driver(0, 0, 320, 210, 0x9B); //draw blue bg
    rectangle_driver(0, 210, 320, 30, 0x80); //draw ground
    draw_sun(10,15);
    draw_daytime_cloud(15,50);
    draw_daytime_cloud(85,35);
    draw_daytime_cloud(115,40);
    draw_daytime_cloud(145,30);
    draw_daytime_cloud(175,20);
    draw_daytime_cloud(225,10);
    draw_daytime_cloud(255,25);
    draw_daytime_cloud(277,25);
    draw_daytime_cloud(290,35);
    draw_daytime_tree(30,210-27);
    draw_daytime_tree(80,210-27);
    draw_daytime_tree(100,210-27);
    draw_daytime_tree(120,210-27);
    draw_daytime_tree(150,210-27);
    draw_daytime_tree(180,210-27);
    draw_daytime_tree(220,210-27);
    draw_daytime_tree(250,210-27);
    draw_daytime_tree(270,210-27);
    draw_daytime_tree(295,210-27);
}

/*draw one moon per time*/
void draw_moon(int x, int y){
    rectangle_driver(x, y, 12, 12, 0xFE);
    rectangle_driver(x+3, y+3, 7, 1, 0xF9);
    rectangle_driver(x+3, y+5, 5, 1, 0xF9);
    rectangle_driver(x+3, y+9, 7, 1, 0xF9);
    rectangle_driver(x+6, y+7, 2, 1, 0xF9);
    rectangle_driver(x+3, y+6, 1, 3, 0xF9);
    rectangle_driver(x+7, y+6, 1, 1, 0xF9);
    rectangle_driver(x+9, y+4, 1, 5, 0xF9);
}

/*draw one star per time*/
void draw_star(int x, int y){
    rectangle_driver(x, y, 1, 1, 0xF9);
    rectangle_driver(x-1, y+1, 3, 2, 0xF9);
    rectangle_driver(x-5, y+3, 11, 1, 0xF9);
    rectangle_driver(x-4, y+4, 9, 1, 0xF9);
    rectangle_driver(x-2, y+5, 5, 2, 0xF9);
    rectangle_driver(x-3, y+7, 7, 1, 0xF9);
    rectangle_driver(x-4, y+8, 3, 1, 0xF9);
    rectangle_driver(x+2, y+8, 3, 1, 0xF9);
    //eyes and mouth
    rectangle_driver(x-2, y+4, 1, 1, 0xFF);
    rectangle_driver(x+1, y+4, 1, 1, 0xFF);
    rectangle_driver(x-1, y+4, 1, 1, 0x00);
    rectangle_driver(x+2, y+4, 1, 1, 0x00);
    rectangle_driver(x-1, y+6, 3, 1, 0x00);
    
}

/*draw one cloud per time*/
void draw_night_cloud(int x, int y){
    rectangle_driver(x, y, 5, 1, 0x4D);
    rectangle_driver(x-1, y+1, 8, 2, 0x4D);
    rectangle_driver(x-4, y+3, 14, 4, 0x4D);
    rectangle_driver(x-7, y+7, 19, 2, 0x4D);
    rectangle_driver(x-9, y+9, 23, 6, 0x4D);
    rectangle_driver(x-6, y+15, 15, 1, 0x4D);
    rectangle_driver(x-4, y+12, 1, 2, 0xFF);
    rectangle_driver(x+8, y+12, 1, 2, 0xFF);
    rectangle_driver(x-2, y+14, 9, 1, 0xFF);
}

/*draw one tree per time*/
void draw_night_tree(int x, int y){
    rectangle_driver(x, y, 4, 4, 0x12);
    rectangle_driver(x-2, y+4, 8, 4, 0x12);
    rectangle_driver(x-4, y+8, 12, 4, 0x12);
    rectangle_driver(x-6, y+12, 16, 4, 0x12);
    rectangle_driver(x-8, y+16, 20, 4, 0x12);
    rectangle_driver(x-6, y+20, 16, 3, 0x12);
    rectangle_driver(x, y+16, 4, 11, 0x91);
}

/*draw night background*/
void draw_night_bg(){
    rectangle_driver(0, 0, 320, 210, 0x29); //draw blue bg
    rectangle_driver(0, 210, 320, 30, 0x6D); //draw ground
    draw_moon(280,15);
    draw_star(5,15);
    draw_star(15,35);
    draw_star(40,65);
    draw_star(70,45);
    draw_star(85,50);
    draw_star(90,10);
    draw_star(130,5);
    draw_star(150,15);
    draw_star(170,45);
    draw_star(240,26);
    draw_star(255,13);
    draw_star(270,36);
    draw_star(300,48);
    draw_night_cloud(15,50);
    draw_night_cloud(35,20);
    draw_night_cloud(55,10);
    draw_night_cloud(85,35);
    draw_night_cloud(115,40);
    draw_night_cloud(145,30);
    draw_night_cloud(175,20);
    draw_night_cloud(225,10);
    draw_night_cloud(255,25);
    draw_night_cloud(277,25);
    draw_night_cloud(290,35);
    draw_night_tree(20,210-27);
    draw_night_tree(50,210-27);
    draw_night_tree(70,210-27);
    draw_night_tree(100,210-27);
    draw_night_tree(150,210-27);
    draw_night_tree(180,210-27);
    draw_night_tree(220,210-27);
    draw_night_tree(250,210-27);
    draw_night_tree(270,210-27);
    draw_night_tree(295,210-27);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Message Code ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// convert char to predefined int value
int convert_char(char c) {
    int x = 0;
    if (c >= 97 && c <= 122) { // letters a - z lowercase
        x = c - CHARTOINT;
    }
    else if (c >= 48 && c <= 57) { // numbers 0 - 9
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

//draw string centered in screen at specified y value
void draw_string_center_x(int y1, char * string, char colour) {
    int x = (SCREENWIDTH - strlen(string) * CHARWIDTH) / 2;
    draw_string(x, y1, string, colour);
}

//draw string at specified x and y value
void draw_string(int x1, int y1, char * string, char colour) {
    int char_width = CHARWIDTH; //changeable for bigger spacing
    for(int i = 0; i < strlen(string); i++) {
        if (string[i] != ' ') { // skip a spot for space
            char_bp_driver(x1 + char_width * i, y1, convert_char(string[i]), colour);
        }
    }
}

//write number of blocks remaining for status bar UI
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
        // places digits besides blocks and spread out status bar
        if (num_digits == 0) { // if there are no more blocks display zero
            char_bp_driver(block_offset * i + block_offset / 2 + TILESIZE - char_width, GAMEHEIGHT + 3 + STATUSBARHEIGHT, 0, colour);
        }
        else {
            int count = 0;
            for (int j = num_digits - 1; j >= 0; j--) { // used to print multiple digit numbers
                char_bp_driver(block_offset * i + block_offset / 2 + char_width * count + TILESIZE - char_width, 
                                GAMEHEIGHT + 3 + STATUSBARHEIGHT, reverse[j], colour);
                count++;
            }
        }
    }
}

// completion screen rendering
void draw_complete_screen(int time_finished, int blocks_placed) {
    draw_background(1);
    int y = (SCREENHEIGHT - CHARHEIGHT * 3) / 2;
    int time_finished_int = time_finished;
    
    char message[] = "level completed";

    int message_length_timer = 7; // time: .
    int time_finished_copy = time_finished;
    int decimals = time_finished % 1000; // 3 decimal places
    while (time_finished_copy > 0) { // counts how long number is
        message_length_timer++;
        time_finished_copy /= 10;
    }
    char time[message_length_timer];
    sprintf(time, "time: %d.%d", time_finished / 1000, decimals);
    
    int message_length_blocks = 8;
    int blocks_placed_copy = blocks_placed;
    while (blocks_placed_copy > 0) {
        message_length_blocks++;
        blocks_placed_copy /= 10;
    }
    char blocks[message_length_blocks];
    sprintf(blocks, "blocks: %d", blocks_placed);

    // draw items vertically and horizontally centereds
    draw_string_center_x(y, message, 0xFF);
    draw_string_center_x(y + CHARHEIGHT, time, 0xFF);
    draw_string_center_x(y + CHARHEIGHT * 2, blocks, 0xFF);

}

// post completion screen rendering
void draw_post_complete_screen(int mode) {

    int y = (SCREENHEIGHT - CHARHEIGHT * 3) / 2;

    draw_background(1);

    char message1[] = "1: retry";
    char message2[] = "2: next level";
    char message3[] = "3: level select";

    char colour1 = 0xFF;
    char colour2 = 0xFF;
    char colour3 = 0xFF;
    if (mode == 1) {
        colour1 = 0x38; //colour the selected text green
    }
    else if (mode == 2) {
        colour2 = 0x38; //colour the selected text green
    }
    else if (mode == 3) {
        colour3 = 0x38; //colour the selected text green
    }

    // draw text vertically and horizontally centered
    draw_string_center_x(y, message1, colour1);
    draw_string_center_x(y + CHARHEIGHT, message2, colour2);
    draw_string_center_x(y + CHARHEIGHT * 2, message3, colour3);
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
//////////////////////////// Keyboard Drivers ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

char cur_key = '/';

//updates cur_key global variable by polling user input 
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
                cur_key = get_keyvalue(kbd_event->code); // if key value is contained in mapping set curkey to that
            }
            else if (kbd_event->value == 0) {
                cur_key = '/'; // signals no value is being input from keyboard
            }
        }
    }

    close(fd);
    return NULL;
}

//convert int value to char for keyboard input
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
/////////////////////////// Socket Communication /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//create and return socket connection to server
int create_socket() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
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

//sends keyboard input to server and processes game state sent from server to render game
void *game_handler(void *arg) {
    int sock1 = create_socket(); // sender
    int sock2 = create_socket(); // receiver
    int kbd_ptr;
    int ret;
    char key_v;
    char * buffer = malloc(buffer_size);
    int valread;
    int coords_row = 0;
    int coords_col = 0;
    int coords[1000][3] = {0};
    int statusCount[MAXBLOCKS] = {0};

    // signals
    int signal = 1;
    int clear_signal = 0;
    int blockIndex = 0;
    int blockSignal = 0;
    int complete = 0;
    int timeFinish = 0;
    int blocksPlaced = 0;
    int mode = -1;
    int prevMode;
    int bg_mode = 0;

    if(sock1 == -1 || sock2 == -1) {
        printf("connection failed \n");
        return NULL;
    }

    //tell server we are sender socket
	if( send(sock1 , "1", 1, 0) < 0)
	{
		printf("Send failed\n");
		return NULL;
	}

    //tell server we are reciever socket
    if( send(sock2 , "0", 1 , 0) < 0) {
        printf("Send failed\n");
        return NULL;
    }

    while (1) {
        coords_row = 0;
        coords_col = 0;
        if (cur_key != '/') { // key input received
            printf("%c\n", cur_key);
            if( send(sock1 , &cur_key , 1 , 0) < 0) { // send input to server
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
        
        valread = read(sock2, buffer, buffer_size); // read game state from server
        char * token = strtok(buffer, ",");
        
        if (*token == '?') { // question mark complete screen signal and next two items are time to complete and blocks placed
            if (!complete) {
                for (int i = 0; i < 2; i++) {
                    token = strtok(NULL, ",");
                    if (i == 0) {
                        timeFinish = atoi(token);
                    }
                    else if (i == 1) {
                        blocksPlaced = atoi(token);
                    }
                }
                draw_complete_screen(timeFinish, blocksPlaced);
                complete = 1;
            }
        }
        else if (*token == '/') { // forward slash signals post-complete screen with mode as the next item
            token = strtok(NULL, ",");
            prevMode = mode;
            mode = atoi(token);
            if (prevMode != mode) {
                draw_post_complete_screen(mode);
            }
        }
        else {
            while( token != NULL ) {
                complete = 0;
                if (*token == '.') { // period signal clear screen and the next item determines which background to use
                    clear_signal = 1;
                    token = strtok(NULL, ",");
                    bg_mode = atoi(token);
                }
                else if(*token == '&') { // ampersand signals end of coordinates
                    break;
                }
                else if(*token == '!' && blockSignal) { // second exclamation mark we have reached the end of the statusCount
                    blockSignal = 0;
                }
                else if(*token == '!' || blockSignal) { // first exclamation mark signals start of status block count
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
                    // store coordinate data to 2d array with size 3 by x where x is number of tiles
                    int num = atoi(token);
                    coords[coords_row][coords_col] = num;
                    coords_col++;
                    if(coords_col == 3) { // coords_col 0 is the x coordinate, 1 is y coordinate, 2 is the tile type
                        coords_col = 0;
                        coords_row++;
                    }
                }
                token = strtok(NULL, ",");
            }
            
            if (clear_signal) { // reset display if there are block changes
                draw_background(bg_mode);
                clear_signal = 0;
            }

            renderTiles(coords, coords_row); //render tile list sent from server

            if (blockIndex) { //if status bar block amounts are sent, render them
                draw_status_text(statusCount, blockIndex, 0xFF);
            }
        }
        if( send(sock2 , "0", 1, 0) < 0) { //tell server we have finished processing and are ready to receive next game state
            printf("Send failed\n");
            return NULL;
        }

        usleep(50000); // takes only 20 key inputs per second, to match server tick rate
    }

    return NULL;
}

int main(void)
{
    draw_background(1); // clear screen
    pthread_t thread_game;
    pthread_t thread_kb;

    pthread_create(&thread_game, NULL, game_handler, NULL); // server communication thread
    pthread_create(&thread_kb, NULL, listen_kbd, NULL); // thread to poll keyboard inputs

    pthread_join(thread_game, NULL);
    printf("thread 1 joined");
    pthread_join(thread_kb, NULL);
    printf("thread 2 joined");

    return 0;
}