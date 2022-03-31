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
        //printf("Waiting Error\n"); 
        *kbd_ptr = 0;
    }else if(ret == 0){
        //printf("Timeout\n");
        *kbd_ptr = 0;
    }else{
        kbd_event = malloc(sizeof(struct input_event));  
        memset(kbd_event, 0, sizeof(struct input_event));  //clear/reset kbd_event
        ret = read(fd, kbd_event, sizeof(struct input_event));
        if(ret != sizeof(struct input_event)){
        printf("read file %s failed with error %d\n", KBD_EVENT, ret);
        }
        if(kbd_event->type == 0x01){
        *kbd_ptr = kbd_event->code;
        }else if(kbd_event->type == 0x04){
        *kbd_ptr = kbd_event->value;
        }
        // printf("type: 0x%x\n ", kbd_event->type);  
        // printf("code: 0x%x\n ", kbd_event->code);  
        // printf("value: 0x%x\n", kbd_event->value); 
        free(kbd_event);
    }
    close(fd);
    return ret;
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

int main(void)
{
   int kbd_ptr;
   int ret;
   char key_v;
   for(int i = 0; i<50; i++){
       ret = listen_kbd(&kbd_ptr);
       key_v = get_keyvalue(kbd_ptr);
    //    if(ret!=0){
    //     if(kbd_ptr == 0x7001a || kbd_ptr == 0x11){
    //         printf("recieved key: W\n");
    //     }else if(kbd_ptr == 0x70004 || kbd_ptr == 0x1e){
    //         printf("recieved key: A\n");
    //     }else if(kbd_ptr == 0x70016 || kbd_ptr == 0x1f){
    //         printf("recieved key: S\n");
    //     }else if(kbd_ptr == 0x70007 || kbd_ptr == 0x20){
    //         printf("recieved key: D\n");
    //     }
    //    }
    if(ret!=0){
        if(key_v!=0){
            printf("recieved key: %c\n", key_v);
            }
    }
   }

   return 0;
}

//W 0x7001a or 0x11
//A 0x70004 or 0x1e
//S 0x70016 or 0x1f
//D 0x70007 or 0x20
//space 0x7002c or 0x39
//0 0x70027 or 0xb
//1 0x7001e or 0x02
//2 0x7001f or 0x03
//3 0x70020 or 0x04  
//4 0x70021 or 0x05
//5 0x70022 or 0x06
//6 0x70023 or 0x07
//7 0x70024 or 0x08
//8 0x70025 or 0x09
//9 0x70026 or 0xa
//enter 0x70028 or 0x1c
