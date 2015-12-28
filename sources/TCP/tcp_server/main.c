#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h> // для ch_dir
#include<pthread.h>
#include "connection_handler.h"

#define PORT        1234

int main(){
    int sockfd;
    struct sockaddr_in addr, client;
    char dir[MAXBUF] = "/home/user/server/server_work";
    // инициализация рабочей директории
    chdir(dir);

    //Создание сокета TCP
    //AF_INET - стек протоколов TCP/IP, SOXK_STREAM - создание надежного сокета
    //0 - протокол определяется типом сокета

    printf("Создание сокета\n");
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("Ошибка создания сокета");
        exit(errno);
    }

    //Задание адреса и порта серверу
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    //Связь сокета с портом
    printf("bind\n");
    if ( bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0 ){
        perror("ошибка связи сокета");
        exit(errno);
    }


    //включение прослушивания
    // дескриптор сокета, максимальное число очереди
    printf("listen\n");
    if ( listen(sockfd, 1) != 0 ) {
        perror("ошибка включения прослушивания");
        exit(errno);
    }

    puts("Waiting connections");
    int   c = sizeof(struct sockaddr_in);
    int client_sock, *new_sock;
    while( (client_sock = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(4);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}

