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
#define TILE_SIZE 10

volatile char cur_key = '/';

char get_keyvalue(int kbd_ptr);

void * listen_kbd(void * args){
    int* kbd_ptr;
    int fd = -1;
    struct input_event *kbd_event;
    int ret = 0;
    //struct timeval timeout;
    //fd_set event_set;
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
        key_value = 0;
        // default statements
        }
return key_value;
}

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


void *receiver(void *arg) {
    char * buffer = malloc(buffer_size);
    int sock = create_socket();
    int valread;
    if(sock == -1) {
        return NULL;
    }

    if( send(sock , "0", 1, 0) < 0) //used to distinguish what the connection is for on the server
	{
        printf("Send failed");
		return NULL;
	}

    while(1) {
        int coords_row = 0;
        int coords_col = 0;
        int coords[1000][3] = {0}; //change size later

        valread = read(sock, buffer, buffer_size);

        char * token = strtok(buffer, ",");

        while( token != NULL ) {
            if(*token == '&') {
                break;
            }
            int num = atoi(token);
            coords[coords_row][coords_col] = num;
            coords_col++;
            if(coords_col == 3) {
                coords_col = 0;
                coords_row++;
            }
            token = strtok(NULL, ",");
        }

        // for(int i = 0; i < 1000; i++) {
        //     mode3_driver(coords[i][0], coords[i][1], TILE_SIZE, TILE_SIZE, 0x00);
        //     printf("x1: %d, ", coords[i][0]);
        //     printf("y1: %d, ", coords[i][1]);
        //     printf("\n");
        // }
    }
}

void *sender(void *arg) {
    int sock = create_socket();
    int kbd_ptr;
    int ret;
    char key_v;
    char disc_msg[4] = "quit";
    //char message[1024] = {0};
    if(sock == -1) {
        return NULL;
    }

	if( send(sock , "1", 1, 0) < 0)
	{
		printf("Send failed");
		return NULL;
	}

    while (1) {
		// scanf("%s" , message);
        int signal = 1;
        while (1) {
            if (cur_key != '/') {
                printf("%c\n", cur_key);
                if( send(sock , &cur_key , 1 , 0) < 0) { //!!!
                    printf("Send failed");
                    return NULL;
                }
                signal = 1;
            }
            else {
                if (signal) {
                    printf("///////////////////");
                    signal = 0;
                }
            }
            usleep(50000);
        }
    }
}

int main(void)
{
    pthread_t thread_receiver;
    pthread_t thread_sender;
    pthread_t thread_kb;
    pthread_create(&thread_receiver, NULL, receiver, NULL);
    pthread_create(&thread_sender, NULL, sender, NULL);
    pthread_create(&thread_kb, NULL, listen_kbd, NULL);
    pthread_join(thread_receiver, NULL);
    printf("thread 1 joined");
    pthread_join(thread_sender, NULL);
    printf("thread 2 joined");
    pthread_join(thread_kb, NULL);
    printf("thread 3 joined");
    exit(0);

   return 0;
}