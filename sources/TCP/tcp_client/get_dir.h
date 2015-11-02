#ifndef GET_DIR
#define GET_DIR
#include <string.h>

int get_dir(char *buffer, int sock, int buf_size){
     bzero(buffer, buf_size);
     strcpy(buffer, "get_dir\r\n");
     send(sock, buffer, buf_size, 0);

     // прием
     bzero(buffer, buf_size);
     recv(sock, buffer, buf_size,0);
     printf("Текущий каталог: %s\n", buffer);

     return 0;
}
#endif // GET_DIR

