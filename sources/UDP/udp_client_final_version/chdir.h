#ifndef CHDIR
#define CHDIR
#include <string.h>

int ch_dir(char *buffer, int sock, int size, struct sockaddr_in dest_addr, int slen){
	memset(buffer, 0, size);
    strcpy(buffer, "ch_dir\r\n");
    //send(sock, buffer, size, 0);
	sendto(sock, buffer, size, 0, (struct sockaddr*) &dest_addr, slen);

    printf("путь: ");
    memset(buffer, 0, size);
    scanf("%s", buffer);
    //send(sock, buffer, size, 0);
	sendto(sock, buffer, size, 0, (struct sockaddr*) &dest_addr, slen);

    //прием ошибки либо сообщения об успехе
    memset(buffer, 0, size);
    //recv(sock, buffer, size, 0);
	recvfrom(sock, buffer, size, 0, (struct sockaddr *) &dest_addr, &slen);
    if(strcmp(buffer, "succes") != 0)
       printf("Неверный каталог!");

    memset(buffer, 0, size);
    //recv(sock, buffer, size, 0);
	recvfrom(sock, buffer, size, 0, (struct sockaddr *) &dest_addr, &slen);
    printf("Текущий каталог: %s\n", buffer);
    return 0;
}

#endif // CHDIR

