#ifndef CONNECTION_HANDLER
#define CONNECTION_HANDLER
#include<pthread.h>
#include "view.h"
#include "get_dir.h"
#include "view_dir.h"
#include "ch_dir.h"
#include "download.h"
#include "mk_dir.h"
#include "rm_dir.h"
#include "upload.h"

void *connection_handler(void *socket_desc)
{
    //socket
    int sock = *(int*)socket_desc;
    char buffer[MAXBUF];
    char dir[MAXBUF] = "/home/user/server/server_work"; // for "view"

    bzero(buffer, MAXBUF);
    strcpy(buffer, "Connected (server message)\n");
    send(sock, buffer, MAXBUF, 0);

    while(1){
         bzero(buffer, MAXBUF);
         recv(sock, buffer, MAXBUF, 0);

        // Проверка команд
         if (strcmp(buffer, "view\r\n") == 0){
             // send(clientfd, "WORK DIRECTORY/\n", 16, 0);
            view(dir, 5, sock);

            bzero( buffer, MAXBUF);
            sprintf( buffer, "%d", 0);
            strcat(buffer, "|");
            strcat(buffer, "d");
             strcat(buffer, "|");
            strcat(buffer, "end");
            send(sock, buffer, MAXBUF, 0);
         }
        if (strcmp(buffer, "view_dir\r\n") == 0){
            chdir("/home/user/server/server_work");
            view_dir(buffer, MAXBUF, sock);

            bzero( buffer, MAXBUF);
            sprintf( buffer, "%d", 0);
            strcat(buffer, "|");
            strcat(buffer, "d");
            strcat(buffer, "|");
             strcat(buffer, "end");
            send(sock, buffer, MAXBUF, 0);
            chdir("/home/user/server/server_work");
        }
        else if (strcmp(buffer,"disconnect")==0){
            //завершение соединения
            close(sock);
            break;
        }
        else if (strcmp(buffer,"get_dir\r\n")==0){
            get_dir(buffer, MAXBUF, sock);
        }
        else if (strcmp(buffer,"ch_dir\r\n")==0){
            ch_dir(buffer, MAXBUF, sock);
        }
        else if (strcmp(buffer,"upload\r\n")==0){
            download(buffer, MAXBUF, sock);
        }
        else if (strcmp(buffer,"download\r\n")==0){
            upload(buffer, MAXBUF, sock);
        }
        else if (strcmp(buffer,"mk_dir\r\n")==0){
            mk_dir(buffer, MAXBUF, sock, S_IRWXU);
        }
        else if (strcmp(buffer,"rm_dir\r\n")==0){
            rm_dir(buffer, MAXBUF, sock);
        }
        else{
                printf("%s", buffer);
                send(sock, "void \n", 5, 0);
         }
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
#endif // THREAD_HANDLER

