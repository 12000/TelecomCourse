#ifndef RM_DIR
#define RM_DIR
#include <string.h>

int rm_dir(char* buffer, int size, int sock){
    memset(buffer, 0, size);
    strcpy(buffer, "rm_dir\r\n");
    send(sock, buffer, size, 0);

    memset(buffer, 0, size);
    printf("Имя папки: ");
    scanf("%s", buffer);
    printf("\n");
    send(sock, buffer, size, 0);

    memset(buffer, 0, size);
    recv(sock, buffer, size, 0);
    printf("%s", buffer);

    return 0;
}

#endif // RM_DIR
