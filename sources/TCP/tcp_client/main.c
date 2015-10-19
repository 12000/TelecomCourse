#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<math.h>
#include "view.h"

#define MAXBUF  1024

#define PORT    1234

int get_help();
int ch_dir();
int mk_dir();
//int view(char* buffer, int sock, int buf_size);

int main()
{
    int sock;
    struct sockaddr_in server;
    //char message[1000] , server_reply[2000];
    char buffer[MAXBUF];

    //Создание сокета
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 1234 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

        bzero(buffer, MAXBUF);
        recv(sock , buffer , MAXBUF , 0); // прием слова
        puts(buffer);
        while(1){
            bzero(buffer, MAXBUF);
            printf("Cmd: ");
            scanf("%s" , buffer);
             if(strcmp(buffer, "view") == 0)
                    view(buffer, sock, MAXBUF);
             else
                    printf("unknown comand\n");
         }

         close(sock);
         return 0;
}

