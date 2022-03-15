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

#define KBD_EVENT "/dev/input/event0"  

int listen_kbd(int* kbd_ptr){
    int fd = -1;
    int ret = -1;
    struct input_event *kbd_event;
    struct timeval timeout;
    fd_set event_set;
    fd = open(KBD_EVENT, O_RDONLY); 
    if(fd < 0){
        printf("open file %s failed\n", KBD_EVENT);  
        return -1; 
    }
    FD_ZERO(&event_set);
    FD_SET(fd, &event_set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 50000; //microsecond

    ret = select(fd + 1, &event_set, NULL, NULL, &timeout);

    if(ret < 0){
        printf("Waiting Error\n"); 
    }else if(ret == 0){
        printf("Timeout\n");
    }else{
        kbd_event = malloc(sizeof(struct input_event));  
        memset(kbd_event, 0, sizeof(struct input_event));  //clear/reset kbd_event
        ret = read(fd, kbd_event, sizeof(struct input_event));
        if(ret != sizeof(struct input_event)){
        printf("read file %s failed with error %d\n", KBD_EVENT, ret);
        }
        if(kbd_event->type == 0x01){
        *kbd_ptr = kbd_event->code;
        }
        printf("type: 0x%x\n ", kbd_event->type);  
        printf("code: 0x%x\n ", kbd_event->code);  
        printf("value: 0x%x\n", kbd_event->value); 
        free(kbd_event);
    }
    close(fd);
    return ret;
}

    // while(1){
    //     kbd_event = malloc(sizeof(struct input_event));  
    //     memset(kbd_event, 0, sizeof(struct input_event));  //clear/reset kbd_event
    //     ret = read(fd, kbd_event, sizeof(struct input_event));
    //     if(ret != sizeof(struct input_event)){
    //     printf("read file %s failed with error %d\n", KBD_EVENT, ret);
    //     break; 
    //     }
    //     *kbd_ptr = kbd_event->code;
    //     printf("type: 0x%x\n ", kbd_event->type);  
    //     printf("code: 0x%x\n ", kbd_event->code);  
    //     printf("value: 0x%x\n", kbd_event->value); 
    //     free(kbd_event);
    // }