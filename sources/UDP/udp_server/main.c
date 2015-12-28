#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <pthread.h>
#include "view.h"
#include "get_dir.h"
#include "view_dir.h"
#include "ch_dir.h"
#include "download.h"
#include "mk_dir.h"
#include "rm_dir.h"
#include "upload.h"

#define MAXBUF 1024  //Max length of buffer
#define PORT 1234   //The port on which to listen for incoming data
#define MAXCLIENTS 10
typedef struct {
    int sockfd;
    struct sockaddr_in client;
    int clilen;
} btClient;

int numClients = 0;
btClient clients[MAXCLIENTS];
pthread_mutex_t clientsMutex;

btClient acceptConnection(int sockfd);
void* commander(void* cl);
void errorHandler(int err, char *errfun);
void getNewSocket(int* sockfd, uint16_t* port);

int main(void)
{
    //----------------------------------------------------------------------------------------------------
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

    //----------init-------------------------------------------------------------------------------------------
    struct sockaddr_in si_me, si_other;

    int sock, i, slen = sizeof(si_other);
     char dir[MAXBUF] = "/home/user/server/server_work";
     // инициализация рабочей директории
     chdir(dir);
    //create a UDP socket
    if ((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(-1);
    }
    else
        printf("socket created\n");

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(sock, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        perror("bind");
        exit(1);
    }
    else
        printf("bind!\n");
    //--------------------------------------------------------------------------------------------------------------------------------------

    while(1) {
            btClient newClient = acceptConnection(sock);
            printf("new client accept\n");
            pthread_t thread;

            if (pthread_create(&thread, NULL, commander, (void*)&newClient) != 0) {
                printf("Error while creating new thread\n");
            }
            else
                printf("New thread created. Client sock = %d\n", newClient.sockfd);
        }
    return 0;
}


void* commander(void* cl) {
    printf("commander starting\n");
    btClient* client = (btClient*) cl;

    printf("set parameters\n");
    struct sockaddr_in si_other = client->client;
    int sock = client->sockfd;
    int slen = client->clilen;
    sendto(sock, "commander starting\n", sizeof("commander starting\n"), 0, (struct sockaddr*) &client->client, client->clilen);

    char buffer[MAXBUF];
   while(1){
    bzero(buffer, MAXBUF);
    int recv_len;
    if ((recv_len = recvfrom(sock, buffer, MAXBUF, 0, (struct sockaddr *) &si_other, &slen)) == -1)
    {
        perror("recvfrom()");
        exit(-1);
    }
    else
     printf("command was received\n");

     char dir[MAXBUF] = "/home/user/server/server_work";
   // Проверка команд
    printf("%s\n", buffer);
    if (strcmp(buffer, "view\r\n") == 0){

       view(dir, 5, sock, si_other, slen);

       bzero( buffer, MAXBUF);
       sprintf( buffer, "%d", 0);
       strcat(buffer, "|");
       strcat(buffer, "d");
        strcat(buffer, "|");
       strcat(buffer, "end");
       //send(sock, buffer, MAXBUF, 0);
       sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
        chdir("/home/user/server/server_work");
    }
  else if (strcmp(buffer, "view_dir\r\n") == 0){
       chdir("/home/user/server/server_work");
       view_dir(buffer, MAXBUF, sock, si_other, slen);

       bzero( buffer, MAXBUF);
       sprintf( buffer, "%d", 0);
       strcat(buffer, "|");
       strcat(buffer, "d");
       strcat(buffer, "|");
        strcat(buffer, "end");
       //send(sock, buffer, MAXBUF, 0);
        sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
       chdir("/home/user/server/server_work");
   }
   else if (strcmp(buffer,"disconnect")==0){
       //завершение соединения
       close(sock);
      // break;
   }
   else if (strcmp(buffer,"get_dir\r\n")==0){
       get_dir(buffer, MAXBUF, sock, si_other, slen);
   }
   else if (strcmp(buffer,"ch_dir\r\n")==0){
       ch_dir(buffer, MAXBUF, sock, si_other, slen);
   }
   else if (strcmp(buffer,"upload\r\n")==0){
       download(buffer, MAXBUF, sock, si_other, slen);
   }
   else if (strcmp(buffer,"download\r\n")==0){
       upload(buffer, MAXBUF, sock, si_other, slen);
   }
   else if (strcmp(buffer,"mk_dir\r\n")==0){
       mk_dir(buffer, MAXBUF, sock, S_IRWXU, si_other, slen);
   }
   else if (strcmp(buffer,"rm_dir\r\n")==0){
       rm_dir(buffer, MAXBUF, sock, si_other, slen);
   }
   else{
           printf("%s", buffer);
           //send(sock, "void \n", 5, 0);
           if (sendto(sock, "void\n", 5, 0, (struct sockaddr*) &si_other, slen) == -1)
           {
               perror("sendto()");
               exit(1);
           }
    }
    }
    close(client->sockfd);
    return NULL;
}

btClient acceptConnection(int sockfd) {
    struct sockaddr_in clientaddr; /* client addr */
    int clientlen = sizeof(clientaddr); /* byte size of client's address */
    char buf[MAXBUF];
    int err = 0;

    bzero(buf, MAXBUF);
    err=recvfrom(sockfd, buf, MAXBUF, 0, (struct sockaddr *) &clientaddr, &clientlen);
    errorHandler(err, "ERROR in recvfrom");

    int newsockfd;
    uint16_t newport;
    getNewSocket(&newsockfd, &newport);
    bzero(buf, MAXBUF);
    sprintf(buf, "%d", newport);
    err = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &clientaddr, clientlen);
    errorHandler(err, "ERROR in sendto");


    // create new socket for this client
    struct sockaddr_in newclientaddr; /* client addr */
    int newclientlen = sizeof(newclientaddr); /* byte size of client's address */
    bzero(buf, MAXBUF);
    err = recvfrom(newsockfd, buf, MAXBUF, 0, (struct sockaddr *) &newclientaddr, &newclientlen);
    printf("%s\n", buf);
    errorHandler(err, "ERROR in recvfrom");

    btClient client;
    client.sockfd = newsockfd;
    client.client = newclientaddr;
    client.clilen = newclientlen;
    return client;
}

int readFrom(btClient* client, char* message, int len) {
    return recvfrom(client->sockfd,
                    message,
                    len,
                    0,
                    (struct sockaddr *) &client->client,
                    &client->clilen);
}

void errorHandler(int err, char *errfun) {
    if (err < 0) {
        printf("Error in function %s\n", errfun);
        exit(1);
    }
}

void getNewSocket(int* sockfd, uint16_t* port) {
    struct sockaddr_in serveraddr;
    int err = 0;
    *sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    *port = PORT + (numClients + 1);
    errorHandler(*sockfd, "ERROR opening socket");

    int optval = 1;
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port =htons((unsigned short)*port);

    err = bind(*sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
    errorHandler(err, "ERROR on binding");
}
