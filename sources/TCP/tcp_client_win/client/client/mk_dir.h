#ifndef MK_DIR
#define MK_DIR
#include <string.h>

int mk_dir(char* buffer, int size, int sock){
    memset(buffer, 0, size);
    strcpy(buffer, "mk_dir\r\n");
    send(sock, buffer, size, 0);

    memset(buffer, 0, size);
    printf("Имя папки: ");
    scanf("%s", buffer);
    send(sock, buffer, size, 0);

    memset(buffer, 0, size);
    recv(sock, buffer, size, 0);
    printf("%s", buffer);
    return 0;
}

#endif // MK_DIR

