#ifndef GET_DIR
#define GET_DIR
#include <unistd.h>

int get_dir(char *buffer, size_t size, int clientfd){
    bzero( buffer, size);
    getcwd(buffer, size);
    // отправка клиенту
    send(clientfd, buffer, size, 0);
    return 0;
}


#endif // GET_DIR

