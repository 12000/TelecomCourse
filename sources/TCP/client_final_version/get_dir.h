#ifndef GET_DIR
#define GET_DIR
#include <string.h>

int get_dir(char *buffer, int sock, int size){
     memset(buffer, 0, size);
     strcpy(buffer, "get_dir\r\n");
     send(sock, buffer, size, 0);

     // прием
     memset(buffer, 0, size);
     recv(sock, buffer, size,0);
     printf("Текущий каталог: %s\n", buffer);

     return 0;
}
#endif // GET_DIR

