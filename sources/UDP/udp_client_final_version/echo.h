#ifndef ECHO
#define ECHO
#include <string.h>

int echo(char *buffer, int sock, int size, struct sockaddr_in dest_addr, int slen){
	memset(buffer, 0, size);
    scanf("%s", buffer);
    //send(sock, buffer, size, 0);
	sendto(sock, buffer, size, 0, (struct sockaddr*) &dest_addr, slen);

    /*memset(buffer, 0, size);
    //recv(sock, buffer, size, 0);
	recvfrom(sock, buffer, size, 0, (struct sockaddr *) &dest_addr, &slen);
    printf("%s\n", buffer);*/
    return 0;
}

#endif
