#ifndef CH_DIR
#define CH_DIR
#include <unistd.h>
#include <string.h>

int ch_dir(char *buffer, int buf_size, int clientfd){
    bzero(buffer, buf_size);
    recv(clientfd, buffer, buf_size, 0);
    int i;
    i = chdir(buffer);
    /*if(i==0){
        bzero(buffer, buf_size);
        strcpy(buffer, "Успешно\n");
        send(clientfd, buffer, buf_size, 0);
    }
    else{
        bzero(buffer, buf_size);
        strcpy(buffer, "Неудача\n");
        send(clientfd, buffer, buf_size, 0);
    }*/
    return 0;
}

#endif // CH_DIR

