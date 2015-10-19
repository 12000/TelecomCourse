#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include "view.h"

#define PORT        1234

//void view(char *dir, int depth, int clientfd);

int main(){
    int sockfd;
    struct sockaddr_in addr;
    char buffer[MAXBUF];
    char dir[MAXBUF] = "/home/user/server/server_work";
    //char dir[MAXBUF] = "/home/user";

    //Создание сокета TCP
    //AF_INET - стек протоколов TCP/IP, SOXK_STREAM - создание надежного сокета
    //0 - протокло определяется типом сокета

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

 while (1){
        char i;
        // создание нового сокета
        int clientfd;
        struct sockaddr_in client_addr;
        int addrlen=sizeof(client_addr);
        printf("---\n");

        //проверка соединения
        clientfd = accept(sockfd, (struct sockaddr*)&client_addr,&addrlen);
        printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr),
                ntohs(client_addr.sin_port));
        send(clientfd, "Connected\n", 10, 0);

    while(1){
        bzero(buffer, MAXBUF);
        recv(clientfd, buffer, MAXBUF, 0);

        // Проверка команд
       if (strcmp(buffer, "view\r\n") == 0){
          // send(clientfd, "WORK DIRECTORY/\n", 16, 0);
           view(dir, 5, clientfd);

           int depth = 0;
            bzero( buffer, MAXBUF);
            sprintf( buffer, "%d", depth);
            send(clientfd, buffer, MAXBUF, 0);

            //отправка символа папки/файла
             bzero( buffer, MAXBUF);
             strcpy(buffer, "dir");
             send(clientfd, buffer, MAXBUF, 0);

            bzero( buffer, MAXBUF);
            strcpy(buffer, "end");
            send(clientfd, buffer, MAXBUF, 0);
        }
       else if (strcmp(buffer,"disconnect\r\n")==0){
           //завершение соединения
           send(clientfd, "Client Disconnect!\n", 18, 0);
           close(clientfd);
           break;
       }
        else{
             send(clientfd, "void \n", 5, 0);
        }
    }
        break;
}

printf("Сервер остановлен!!!\n");
//Закрытие сокета
close(sockfd);
    return 0;
}

