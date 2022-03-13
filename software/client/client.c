#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// define port we are connecting to
#define PORT 5004
#define buffer_size 960
#define bitmap_width 40
#define bitmap_height 24

int create_socket() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
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
    return sock;
}


void *receiver(void *arg) {
    char buffer[buffer_size] = {0};
    int bitmap_arr[bitmap_height][bitmap_width] = {0};
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
        valread = read( sock , buffer, buffer_size); // Get message sent by server
        int buffer_index = 0;
        for (int i = 0; i < bitmap_height; i++) {
            for (int j = 0; j < bitmap_width; j++) {
                bitmap_arr[i][j] = buffer[buffer_index] - '0';
                buffer_index++;
            }
        }

        for (int i = 0; i < bitmap_height; i++) {
            for (int j = 0; j < bitmap_width; j++) {
                printf("%d, ", bitmap_arr[i][j]);
            }
            printf("\n");
        }
    }
}

void *sender(void *arg) {
    int sock = create_socket();
    char disc_msg[4] = "quit";
    char message[1024] = {0};
    if(sock == -1) {
        return NULL;
    }

	if( send(sock , "1", 1, 0) < 0)
	{
		printf("Send failed");
		return NULL;
	}

    while (1) {
        printf("Enter message : ");
		scanf("%s" , message);
        int flag = 0;

		//Send some data
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			printf("Send failed");
			return NULL;
		}

        // check if client is quitting
        for(int i = 0; i < 4; i++) {
            if(disc_msg[i] != message[i]) {
                flag = 1;
            }
        }

        if (flag == 0) {
            printf("quitting");
			return NULL;
        }
    }
}

int main(int argc, char const *argv[])
{
    pthread_t thread_id1;
    pthread_t thread_id2;
    pthread_create(&thread_id1, NULL, receiver, NULL);
    pthread_create(&thread_id2, NULL, sender, NULL);
    pthread_join(thread_id1, NULL);
    printf("thread 1 joined");
    pthread_join(thread_id2, NULL);
    printf("thread 2 joined");
    exit(0);

    return 0;
}
