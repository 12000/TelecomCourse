#include <stdio.h>

#include <stdbool.h>

#include <sys/socket.h>

#include <resolv.h>

#include <arpa/inet.h>

#include <errno.h>

#include <string.h>

#include <unistd.h>

#include <dirent.h>

#include <sys/stat.h>

#include <stdlib.h>

#define MAXBUF		1024

#define PORT        1234

void printdir(char *dir, int depth, int clientfd);

int main()

{   int sockfd;

    struct sockaddr_in addr;

    char buffer[MAXBUF];

    //char dir[MAXBUF] = "/home/user/server/server_work";
    char dir[MAXBUF] = "/home/user";

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

    if ( bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )

    {

        perror("ошибка связи сокета");

        exit(errno);

    }


    //включение прослушивания
    // дескриптор сокета, максимальное число очереди

    printf("listen\n");

    if ( listen(sockfd, 1) != 0 )

    {

        perror("ошибка включения прослушивания");

        exit(errno);

    }

    //char quit[MAXBUF] = "quit";

    //char tmp_buf[MAXBUF];

//    while (1){

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


        //bool connected = true;

        send(clientfd, "Connected\n", 10, 0);

        bzero(buffer, MAXBUF);

        recv(clientfd, buffer, MAXBUF, 0);

       if (strcmp(buffer, "view\r\n") == 0){

           send(clientfd, "WORK DIRECTORY/\n", 16, 0);

           printdir(dir, 5, clientfd);

        }
        else{

             send(clientfd, "    \n", 5, 0);
        }

        //завершение соединения
        close(clientfd);

//}

printf("Сервер остановлен!!!\n");

/*---Закрытие сокета---*/

close(sockfd);

    return 0;

}


void printdir(char *dir, int depth, int clientfd) {

    // буфер для послыки
char buffer[MAXBUF];

 DIR *dp;

 struct dirent *entry;

 struct stat statbuf;

 if ((dp = opendir(dir)) == NULL) {

             fprintf(stderr, "cannot open directory: %s\n", dir);

             return;

 }

 chdir(dir);

 while((entry = readdir(dp)) != NULL) {

        lstat(entry->d_name, &statbuf);

         if (S_ISDIR(statbuf.st_mode)) {

        /* Находит каталог, но игнорирует . и .. */

                if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)

                        continue;

                 //printf("%*s%s/\n", depth, "", entry->d_name);

                bzero( buffer, MAXBUF);

                int i=0;

                for(i; i<depth; i++)
                    send(clientfd, " ", 1, 0);

                send(clientfd, buffer, MAXBUF, 0);

                 bzero( buffer, MAXBUF);

                 strcpy(buffer, entry->d_name);

                 send(clientfd, buffer, MAXBUF, 0);

                 bzero( buffer, MAXBUF);

                 send(clientfd, "/\n", 2, 0);

                /* Рекурсивный вызов с новый отступом */

                printdir(entry->d_name, depth+6, clientfd);

        }
         else {
             //printf("%*s%s\n", depth, " ", entry->d_name);

             bzero( buffer, MAXBUF);

             int i=0;

             for(i; i<depth; i++)
                        send(clientfd, " ", 1, 0);

             send(clientfd, buffer, MAXBUF, 0);

              bzero( buffer, MAXBUF);

              strcpy(buffer, entry->d_name);

              send(clientfd, buffer, MAXBUF, 0);

              bzero( buffer, MAXBUF);

              send(clientfd, "\n", 1, 0);

         }

 }

 chdir("..");

 closedir(dp);

}
