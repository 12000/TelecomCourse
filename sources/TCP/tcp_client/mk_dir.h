#ifndef MK_DIR
#define MK_DIR
#include <string.h>

int mk_dir(char* buffer, int size, int sock){
    bzero(buffer, size);
    strcpy(buffer, "mk_dir\r\n");
    send(sock, buffer, size, 0);

    bzero(buffer, size);
    printf("Имя папки: ");
    scanf("%s", buffer);
    send(sock, buffer, size, 0);

    bzero(buffer, size);
    recv(sock, buffer, size, 0);
    printf("%s", buffer);
    return 0;
}

#endif // MK_DIR

