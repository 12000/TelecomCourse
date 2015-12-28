#ifndef ECHO
#define ECHO
#include <string.h>

int echo(char *buffer, int sock, int size){
	memset(buffer, 0, size);
    scanf("%s", buffer);
    send(sock, buffer, size, 0);

    memset(buffer, 0, size);
    recv(sock, buffer, size, 0);
    printf("%s\n", buffer);
    return 0;
}

#endif
