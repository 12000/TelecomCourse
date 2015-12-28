#ifndef MK_DIR
#define MK_DIR
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int mk_dir(char* buffer, int size, int sock, mode_t mode, struct sockaddr_in si_other, int slen){
    int s;
    bzero(buffer, size);
    //recv(sock, buffer, size, 0);
    recvfrom(sock, buffer, MAXBUF, 0, (struct sockaddr *) &si_other, &slen);
    s = mkdir(buffer, mode);
    if(s == 0){
        bzero(buffer, size);
        strcpy(buffer,"Directory created\n");
        //send(sock, buffer, size, 0);
        sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
    }
    else{
        bzero(buffer, size);
        strcpy(buffer,"Directory don't created\n");
        //send(sock, buffer, size, 0);
        sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
    }

    return 0;
}

#endif // MK_DIR

