#ifndef CH_DIR
#define CH_DIR
#include <unistd.h>
#include <string.h>

int ch_dir(char *buffer, int size, int sock, struct sockaddr_in si_other, int slen){
    bzero(buffer, size);
    recv(sock, buffer, size, 0);
    int i;
    char dir[256] = "/home/user/server/server_work";
    strcat(dir, buffer);
    i = chdir(dir);
    if (i == -1){
        bzero(buffer, size);
        strcpy(buffer, "Недействительный путь!");
        //send(sock, buffer, size, 0);
        sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);

        bzero(buffer, size);
        getcwd(buffer, size);
        //send(sock, buffer, size, 0);
         sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
    }
    else{
        bzero(buffer, size);
        strcpy(buffer, "succes");
        //send(sock, buffer, size, 0);
         sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);

        bzero(buffer, size);
        getcwd(buffer, size);
        //send(sock, buffer, size, 0);
         sendto(sock, buffer, MAXBUF, 0, (struct sockaddr*) &si_other, slen);
    }
    printf("%s\n", dir);
    return 0;
}

#endif // CH_DIR

