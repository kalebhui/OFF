#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <signal.h>
#include <limits.h>
#include <linux/input.h>
#include <pthread.h>

#define KBD_EVENT "/dev/input/event0"

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
    // FD_ZERO(&event_set);
    // FD_SET(fd, &event_set);

    // timeout.tv_sec = 0;
    // timeout.tv_usec = 50000; //microsecond

    // ret = select(fd + 1, &event_set, NULL, NULL, &timeout);
    while (1) {
        // if(ret < 0){
        //     //printf("Waiting Error\n");
        //     *kbd_ptr = 0;
        // }else if(ret == 0){
        //     //printf("Timeout\n");
        //     *kbd_ptr = 0;
        //}else{

        kbd_event = malloc(sizeof(struct input_event));
        memset(kbd_event, 0, sizeof(struct input_event));  //clear/reset kbd_event
        ret = read(fd, kbd_event, sizeof(struct input_event));

        if(ret != sizeof(struct input_event)){
            printf("read file %s failed with error %d\n", KBD_EVENT, ret);
        }
        // if (kbd_event->value == 1) {
        //     cur_key = get_keyvalue(kbd_event->code);
        // }
        // else {
        //     cur_key = '/';
        // }
        printf("type: 0x%x\n ", kbd_event->type);
        printf("code: 0x%x\n ", kbd_event->code);
        printf("value: 0x%x\n", kbd_event->value);
        if(kbd_event->type == 0x01){
            if (kbd_event->value == 1) {
                cur_key = get_keyvalue(kbd_event->code);
            }
            else if (kbd_event->value == 0) {
                cur_key = '/';
            }
        }
        // }else if(kbd_event->type == 0x04){ //???
        //     // *kbd_ptr = kbd_event->value;
            
        // }

            // printf("type: 0x%x\n ", kbd_event->type);
            // printf("code: 0x%x\n ", kbd_event->code);
            // printf("value: 0x%x\n", kbd_event->value);
            //free(kbd_event);
        //}
    }

    close(fd);
    // return ret;
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
        key_value = '/';
        // default statements
        }
return key_value;
}

void *printer(void *arg) {
    int signal = 1;
    while (1) {
        if (cur_key != '/') {
            printf("%c \n", cur_key);
            signal = 1;
        }
        else {
            if (signal) {
                printf("//////////////////\n");
                signal = 0;
            }
        }
        usleep(50000);
    }
}

int main(void)
{
    pthread_t thread_id1;
    pthread_t thread_id2;
    pthread_create(&thread_id1, NULL, listen_kbd, NULL);
    pthread_create(&thread_id2, NULL, printer, NULL);
    pthread_join(thread_id1, NULL);
    printf("thread 1 joined");
    pthread_join(thread_id2, NULL);
    printf("thread 2 joined");
    exit(0);

   return 0;
}
