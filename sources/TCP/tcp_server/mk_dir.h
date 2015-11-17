#ifndef MK_DIR
#define MK_DIR
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int mk_dir(char* buffer, int size, int sock, mode_t mode){
    int s;
    bzero(buffer, size);
    recv(sock, buffer, size, 0);
    s = mkdir(buffer, mode);
    if(s == 0){
        bzero(buffer, size);
        strcpy(buffer,"Directory created\n");
        send(sock, buffer, size, 0);
    }
    else{
        bzero(buffer, size);
        strcpy(buffer,"Directory don't created\n");
        send(sock, buffer, size, 0);
    }

    return 0;
}

#endif // MK_DIR

