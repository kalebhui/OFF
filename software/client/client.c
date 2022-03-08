// Client side C program (adapted from https://www.geeksforgeeks.org/socket-programming-cc/)
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

// define port we are connecting to
#define PORT 10007
#define buffer_size 960
#define bitmap_width 40
#define bitmap_height 24

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    // int bitmap_width = 40;
    // int bitmap_height = 24;
    struct sockaddr_in serv_addr;
    // int buffer_size = 40*24;
    char buffer[buffer_size] = {0};
    int bitmap_arr[bitmap_height][bitmap_width] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET; // For IPv6
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
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

    printf("Connection Successful\n");
    valread = read( sock , buffer, buffer_size); // Get message sent by server

    printf("%s\n",buffer);               // print message

    int buffer_index = 0;
    for (int i = 0; i < bitmap_height; i++) {
        for (int j = 0; j < bitmap_width; j++) {
            bitmap_arr[i][j] = buffer[buffer_index] - '0';
            buffer_index++;
        }
    }

    // for (int i = 0; i < bitmap_height; i++) {
    //     for (int j = 0; j < bitmap_width; j++) {
    //         printf("%d, ", bitmap_arr[i][j]);
    //     }
    //     printf("\n");
    // }


    return 0;
}
