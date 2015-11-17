#ifndef UPLOAD
#define UPLOAD
#include <string.h>
#include <stdio.h>

int upload(char* buffer, int size, int sock){
    char name[size];
    int fileSize;
    int message_length;

    FILE* f;

    printf("Путь: ");
    bzero(name, size);
    scanf("%s", name);

    f = fopen(name, "rb");
    if(f == NULL){
        printf("File dont open\n");
        return -1;
    }

    bzero(buffer, size);
    strcpy(buffer, "upload\r\n");
    send(sock, buffer, size, 0);

    // отправка на сервер имени файла
    printf("Имя на сервере: ");
    bzero(buffer, size);
    scanf("%s",buffer);
    send(sock, buffer, size, 0);

    //отправка размера
    fseek(f, 0, SEEK_END);
    fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("Size of file: %d\n", fileSize);
    bzero(buffer, size);
    //strcpy(buffer, (char)fileSize);
    sprintf(buffer, "%d", fileSize);
    send(sock, buffer, size, 0);

    while(fileSize > 0){
        bzero(buffer, size);
        if(fileSize >=1024){
            message_length = fread(buffer, size, 1, f); // количество

            if(message_length != 0)
                    send(sock, buffer, size, 0);
            bzero(buffer, size);
            recv(sock, buffer, size, 0);
            printf("Передача - было принято: %s\n", buffer);
        }
        else{
            message_length = fread(buffer, fileSize, 1, f);

            if(message_length != 0)
                    send(sock, buffer, fileSize, 0);
            bzero(buffer, size);
            recv(sock, buffer, size, 0);
            printf("Передача - было принято: %s\n", buffer);
        }
        fileSize -= 1024;
    }
    fclose(f);
    return 0;
}

#endif // UPLOAD

