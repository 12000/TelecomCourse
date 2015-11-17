#ifndef RM_DIR
#define RM_DIR
#include <unistd.h>

int rm_dir(char* buffer, int size, int sock){
    int s;
    bzero(buffer, size);
    recv(sock, buffer, size, 0);
    s = rmdir(buffer);

    if(s == 0){
        bzero(buffer, size);
        strcpy(buffer,"Directory removed\n");
        send(sock, buffer, size, 0);
    }
    else{
        bzero(buffer, size);
        strcpy(buffer,"Directory don't removed\n");
        send(sock, buffer, size, 0);
    }

    return 0;
}
#endif // RM_DIR

