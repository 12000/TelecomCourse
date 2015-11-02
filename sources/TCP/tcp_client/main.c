#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<math.h>
#include "view.h"
#include "help.h"
#include "get_dir.h"
#include "view_dir.h"
#include "chdir.h"

#define MAXBUF  1024
#define PORT    1234

int main()
{
    int sock;
    struct sockaddr_in server;

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
        printf("Для получения справки введите команду help\n");
        while(1){
            bzero(buffer, MAXBUF);
            printf("Cmd: ");
            scanf("%s" , buffer);
            // команды
             if(strcmp(buffer, "view") == 0)
                    view_dir(buffer, sock, MAXBUF);
             else if(strcmp(buffer, "view_dir") == 0)
                    view_dir(buffer, sock, MAXBUF);
             else if(strcmp(buffer, "help") == 0)
                     get_help();
             else if(strcmp(buffer, "get_dir") == 0)
                    get_dir(buffer, sock, MAXBUF);
             else if(strcmp(buffer, "ch_dir") == 0)
                    ch_dir(buffer, sock, MAXBUF);
             else
                    printf("unknown comand\n");
             printf("Для получения справки введите команду help\n");
         }

         close(sock);
         return 0;
}

