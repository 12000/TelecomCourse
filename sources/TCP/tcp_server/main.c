#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h> // для ch_dir
#include "view.h"
#include "get_dir.h"
#include "view_dir.h"
#include "ch_dir.h"
#include "download.h"
#include "mk_dir.h"
#include "rm_dir.h"

#define PORT        1234

//void view(char *dir, int depth, int clientfd);

int main(){
    int sockfd;
    struct sockaddr_in addr;
    char buffer[MAXBUF];
    char dir[MAXBUF] = "/home/user/server/server_work";
    // инициализация рабочей директории
    //chdir("/home/user/server/server_work");
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
        send(clientfd, "Connected (server message)\n", 28, 0);

        while(1){

             bzero(buffer, MAXBUF);
             recv(clientfd, buffer, MAXBUF, 0);

            // Проверка команд
             if (strcmp(buffer, "view\r\n") == 0){
                 // send(clientfd, "WORK DIRECTORY/\n", 16, 0);
                view(dir, 5, clientfd);

                bzero( buffer, MAXBUF);
                sprintf( buffer, "%d", 0);
                strcat(buffer, "|");
                strcat(buffer, "d");
                 strcat(buffer, "|");
                strcat(buffer, "end");
                send(clientfd, buffer, MAXBUF, 0);
             }
            if (strcmp(buffer, "view_dir\r\n") == 0){
                chdir("/home/user/server/server_work");
                view_dir(buffer, MAXBUF, clientfd);

                bzero( buffer, MAXBUF);
                sprintf( buffer, "%d", 0);
                strcat(buffer, "|");
                strcat(buffer, "d");
                strcat(buffer, "|");
                 strcat(buffer, "end");
                send(clientfd, buffer, MAXBUF, 0);
                chdir("/home/user/server/server_work");
            }
            else if (strcmp(buffer,"disconnect")==0){
                //завершение соединения
                send(clientfd, "Client Disconnect!\n", 18, 0);
                close(clientfd);
                break;
            }
            else if (strcmp(buffer,"get_dir\r\n")==0){
                get_dir(buffer, MAXBUF, clientfd);
            }
            else if (strcmp(buffer,"ch_dir\r\n")==0){
                ch_dir(buffer, MAXBUF, clientfd);
            }
            else if (strcmp(buffer,"upload\r\n")==0){
                download(buffer, MAXBUF, clientfd);
            }
            else if (strcmp(buffer,"mk_dir\r\n")==0){
                mk_dir(buffer, MAXBUF, clientfd, S_IRWXU);
            }
            else if (strcmp(buffer,"rm_dir\r\n")==0){
                rm_dir(buffer, MAXBUF, clientfd);
            }
            else{
                    printf("%s", buffer);
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

