#ifndef UPLOAD
#define UPLOAD
#include <string.h>
#include <stdio.h>

int upload(char* buffer, int size, int sock){
    char name[1024], trace[1024];
    int fileSize;
    int message_length;
    FILE* f;

    //прием имени файла
    bzero(name, size);
    recv(sock, name, size, 0);

    //формирование пути открытия файла
    bzero(trace, 1024);
    getcwd(trace, size);
    strcat(trace, "/");
    strcat(trace, name);

    f = fopen(name, "rb");
    if(f == NULL){
        printf("File dont open\n");
        bzero(buffer, size);
        strcpy(buffer, "false");
        send(sock, buffer, size, 0);
        return -1;
    }
    else{
        bzero(buffer, size);
        strcpy(buffer, "true");
        send(sock, buffer, size, 0);
    }


    //отправка размера
    fseek(f, 0, SEEK_END);
    fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("Size of file: %d\n", fileSize);
    memset(buffer, 0, size);
    //strcpy(buffer, (char)fileSize);
    sprintf(buffer, "%d", fileSize);
    send(sock, buffer, size, 0);


    while(fileSize > 0){
        memset(buffer, 0, size);
        if(fileSize >=1024){
            message_length = fread(buffer, size, 1, f); // количество

            if(message_length != 0)
                    send(sock, buffer, size, 0);
        }
        else{
            message_length = fread(buffer, fileSize, 1, f);

            if(message_length != 0)
                    send(sock, buffer, fileSize, 0);
        }
        fileSize -= 1024;
    }
    fclose(f);

    return 0;
}

#endif // UPLOAD

