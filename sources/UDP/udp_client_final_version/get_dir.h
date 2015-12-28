#ifndef GET_DIR
#define GET_DIR
#include <string.h>

int get_dir(char *buffer, int sock, int size, struct sockaddr_in dest_addr, int slen){
     memset(buffer, 0, size);
     strcpy(buffer, "get_dir\r\n");
     //send(sock, buffer, size, 0);
	 sendto(sock, buffer, size, 0, (struct sockaddr*) &dest_addr, slen);

     // прием
     memset(buffer, 0, size);
     //recv(sock, buffer, size,0);
	 recvfrom(sock, buffer, size, 0, (struct sockaddr *) &dest_addr, &slen);
     printf("Текущий каталог: %s\n", buffer);

     return 0;
}
#endif // GET_DIR

