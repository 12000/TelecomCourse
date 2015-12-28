#ifndef RM_DIR
#define RM_DIR
#include <unistd.h>

int rm_dir(char* buffer, int size, int sock, struct sockaddr_in si_other, int slen){
    int s;
    bzero(buffer, size);
    //recv(sock, buffer, size, 0);
    recvfrom(sock, buffer, MAXBUF, 0, (struct sockaddr *) &si_other, &slen);
    s = rmdir(buffer);

    if(s == 0){
        bzero(buffer, size);
        strcpy(buffer,"Directory removed\n");
        //send(sock, buffer, size, 0);
        sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
    }
    else{
        bzero(buffer, size);
        strcpy(buffer,"Directory don't removed\n");
        //send(sock, buffer, size, 0);
        sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
    }

    return 0;
}
#endif // RM_DIR

