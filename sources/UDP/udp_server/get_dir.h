#ifndef GET_DIR
#define GET_DIR
#include <unistd.h>

int get_dir(char *buffer, size_t size, int clientfd, struct sockaddr_in si_other, int slen){
    bzero( buffer, size);
    getcwd(buffer, size);
    // отправка клиенту
    //send(clientfd, buffer, size, 0);
     sendto(clientfd, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
    return 0;
}


#endif // GET_DIR

