#ifndef UPLOAD
#define UPLOAD
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int upload(char* buffer, int size, int sock, struct sockaddr_in si_other, int slen){
    char name[2048], trace[2048];
    int fileSize;
    int message_length;
    FILE* f;

    //прием имени файла
    bzero(name, size);
    //recv(sock, name, size, 0);
    int r = recvfrom(sock, name, size, 0, (struct sockaddr *) &si_other, &slen);
    printf("%d\n", r);

    //формирование пути открытия файла
    bzero(trace, size);
    getcwd(trace, size);
    strcat(trace, "/");
    strcat(trace, name);
    printf("%s\n", trace);

    f = fopen(name, "rb");
    if(f == NULL){
        printf("File dont open\n");
        bzero(buffer, size);
        strcpy(buffer, "false");
        //send(sock, buffer, size, 0);
        sendto(sock, buffer, size, 0, (struct sockaddr*) &si_other, slen);
        sleep(1);
        return -1;
    }
    else{
        bzero(buffer, size);
        strcpy(buffer, "true");
        //send(sock, buffer, size, 0);
        sendto(sock, buffer, size, 0, (struct sockaddr*) &si_other, slen);
        sleep(1);
    }


    //отправка размера
    fseek(f, 0, SEEK_END);
    fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("Size of file: %d\n", fileSize);
    bzero(buffer, size);
    //strcpy(buffer, (char)fileSize);
    sprintf(buffer, "%d", fileSize);
    //send(sock, buffer, size, 0);
    sendto(sock, buffer, size, 0, (struct sockaddr*) &si_other, slen);
    sleep(1);

    while(fileSize > 0){
        bzero(buffer, size);
        if(fileSize >=1024){
            message_length = fread(buffer, size, 1, f); // количество

            if(message_length != 0)
                    //send(sock, buffer, size, 0);
                    sendto(sock, buffer, size, 0, (struct sockaddr*) &si_other, slen);
                    sleep(1);
        }
        else{
            message_length = fread(buffer, fileSize, 1, f);

            if(message_length != 0)
                    //send(sock, buffer, fileSize, 0);
                    sendto(sock, buffer, fileSize, 0, (struct sockaddr*) &si_other, slen);
                    sleep(1);
        }
        fileSize -= size;
    }
    fclose(f);

    return 0;
}

#endif // UPLOAD
